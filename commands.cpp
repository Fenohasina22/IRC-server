/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/20 14:33:38 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

bool	passCmd(Client &client, iRCMessage &mess, Server &serv, bool &validPass)
{
    if (client.isRegistered())
    {
		client.ConcatenateWBuffer(FormatedMessage("462", ":" + serv.getName(),
			 "* :You may not reregister"), serv);
		return (false);
	}
	if (mess.args.empty())
    {
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 "* PASS :Not enough parameters"), serv);
		return (false);
	}
	if (mess.args[0] != serv.getPass())
    {
		client.ConcatenateWBuffer(FormatedMessage("464", ":" + serv.getName(),
			 "* :Password incorrect"), serv);
		client.setPendingClose(true);
		return (false);
	}
    client.setPassState(true);
	validPass = true;
	serv.getTrueClients().push_back(client);
	return (true);
}

bool	nickCmd(Client &client, iRCMessage &mess, Server &serv)
{
	std::string newNick;

	if (!client.getPassState())
	{
		client.ConcatenateWBuffer(FormatedMessage("451", ":" + serv.getName(),
		 "* :You have not registered"), serv);
		return (false);
	}
	if (mess.args.empty())
    {
		client.ConcatenateWBuffer(FormatedMessage("431", ":" + serv.getName(),
			 "* :No nickname specified"), serv);
		return (false);
	}
	newNick = mess.args[0];
	if (!newNick.empty() && newNick[0] == ':')
		newNick = newNick.substr(1);
	if (!IsValidNick(newNick, client, serv))
		return (false);
	if (isNicknameInUse(serv, client, newNick))
		return (false);
	if (!notifyNeighbors(client, serv, newNick))
		return (false);
	if (!updateChannels(client, serv, newNick))
		return (false);
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client, serv);
	return (true);
}

bool	userCmd(Client &client, iRCMessage &mess, Server& serv)
{
	std::string newUser;

	if (!client.getPassState())
	{
		client.ConcatenateWBuffer(FormatedMessage("451", ":" + serv.getName(),
		 "* :You have not registered"), serv);
		return (false);
	}
	if (client.isRegistered())
    {
		client.ConcatenateWBuffer(FormatedMessage("462", ":" + serv.getName(),
			 "* :You may not reregister"), serv);
		return (false);
	}
	if (mess.args.size() < 4)
    {
		client.ConcatenateWBuffer(FormatedMessage("461" , ":" + serv.getName(),
			 "* USER :Not enough parameters"), serv);
		return (false);
	}
	newUser = mess.args[0];
	if (!newUser.empty() && newUser[0] == ':')
		newUser = newUser.substr(1);
    client.setUser(newUser);
    client.setReal(mess.args[3]);
    client.setUserState(true);
    tryRegistration(client, serv);
	return (true);
}

bool	capCmd(Client &client, iRCMessage &mess, Server& serv)
{
	if (!mess.args.empty() && mess.args[0] == "LS")
		client.ConcatenateWBuffer(":" + serv.getName() + " CAP * LS :\r\n", serv);
	return (true);
}

bool	pongCmd(Client &client, iRCMessage &mess, Server& serv)
{
	std::string pongstr;

	if (mess.args.empty())
		pongstr = std::string("PONG") + CRLF;
	else
		pongstr = "PONG " + mess.args[0] + CRLF;
	client.ConcatenateWBuffer(pongstr, serv);
	std::cout << GREEN << "PING handled for fd:" << client.getFd() << RESET << std::endl;
	return (true);
}

bool	privmsgCmd(Client &client, iRCMessage &mess, Server &serv)
{
	if (privmsgError(mess, client, serv))
		return (false);
	std::vector<std::string>	vec = split(mess.args[0], ',');
	unsigned int				i	= 0;

	while (i < vec.size())
	{
		bool			foundClient 	= false;
		bool			foundChan	 	= false;
		Client			&destCli	 	= serv.findTrueClient(vec[i], foundClient);
		Channel			&destChan		= serv.findChan(vec[i], foundChan);
		Client 			&sender 		= client;
		std::string 	messageOutput;

		if (!foundClient && !foundChan)
		{
			client.ConcatenateWBuffer(FormatedMessage("401", ":" + serv.getName(),
				 client.getNick() + " " + vec[i] + " :No such nick/channel"), serv);
			return (false);
		}
		if (foundClient)
		{
			messageOutput = formMess(sender, destCli, mess);
			destCli.ConcatenateWBuffer(messageOutput, serv);
		}
		else
		{
			privmsgToChan(sender, destChan, serv, mess, messageOutput);
		}
		i++;
	}
	return (true);
}

bool	joinCmd(Client &client, iRCMessage &mess, Server &serv)
{
	Channel 					*destChan;
	bool						foundChan	= false;
	std::string 				broadcastMess;
	std::vector<std::string>	vec;
	unsigned int				idx			= 0;

	if (mess.args.empty())
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 "* JOIN :Not enough parameters"), serv);
		return (false);
	}
	while (idx < mess.args.size())
	{
		vec = split(mess.args[idx],',');
		unsigned int i;
		i = 0;
		while (i < vec.size())
		{
			vec[i] = strtrim(vec[i]);
			if (!chanExists(vec[i], serv))
			{
				if (vec[i][0] != '#')
				{
					client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
						 client.getNick() + " " + vec[i] + " :No such channel"), serv);
					return (false);
				}
				serv.getAllChans().push_back(Channel(vec[i], ""));
				std::cout << GREEN << "Channel created: " << vec[i] << RESET << std::endl;
				destChan = &(serv.getAllChans().back());
			}
			else
				destChan = &(serv.findChan(vec[i], foundChan));
			if (!checkChanRestrictions(client, serv, mess, destChan))
				return (false);
			destChan->addClient(&client);
			std::cout << GREEN << client.getNick() << " joined " << destChan->getName() << RESET << std::endl;
			if (destChan->getMembers().size() == 1)
				destChan->addOperator(&client);
			broadcastJoin(broadcastMess, client, serv, destChan);
			if (destChan->isInvited(client.getNick()))
				destChan->removeInvited(&client);
			sendChannelState(client, *destChan, serv);
			i++;
		}
		idx++;
	}

	return (true);
}

bool	partCmd(Client &client, iRCMessage &mess, Server &serv)
{
	std::string broadcastMess;
	bool		foundChan;

	if (mess.args.empty())
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 "* PART :Not enough parameters"), serv);
		return (false);
	}
	if (!chanExists(mess.args[0], serv))
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (!client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
		return (false);
	}
	Channel &destChan = serv.findChan(mess.args[0], foundChan);

	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@" + CurrentHostname(client);
	broadcastMess += " PART ";
	broadcastMess += destChan.getName();
	if (mess.args.size() > 1)
		broadcastMess += mess.args[1];
	broadcastMess += CRLF;
	std::string reason = (mess.args.size() > 1) ? mess.args[1] : "";
	std::cout << GREEN << "PART: " << client.getNick() << " -> " << destChan.getName()
		<< (reason.empty() ? "" : std::string(" reason: ") + reason) << RESET << std::endl;
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, destChan, serv);
		destChan.removeClient(&client);
		std::cout << GREEN << client.getNick() << " parted " << destChan.getName() << RESET << std::endl;
		if (destChan.getMembers().size() == 0)
			serv.deleteChan(mess.args[0]);
	return (true);
}

bool	topicCmd(Client &client, iRCMessage &mess, Server &serv)
{
	bool		displayTopic	= false;
	bool		changeTopic		= false;
	bool		resetTopic		= false;
	bool 		foundChan		= false;
	std::string	broadcastMess;

	if (mess.args.empty())
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 "* TOPIC :Not enough parameters"), serv);
		return (false);
	}
	Channel &destChan = serv.findChan(mess.args[0], foundChan);

	if (!foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (!processTopicCommand(displayTopic, changeTopic, resetTopic,
		 client, mess, serv, destChan))
		return (false);
	updateTopic(changeTopic, resetTopic, mess, destChan, broadcastMess, client, serv);
	return (true);
}

bool	kickCmd(Client &client,iRCMessage &mess,Server &serv)
{
	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 "* KICK :Not enough parameters"), serv);
		return (false);
	}
	if (!chanExists(mess.args[0], serv))
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}

	bool		foundCli 	= false;
	bool		foundChan	= false;
	Client		&destCli	= serv.findTrueClient(mess.args[1], foundCli);
	Channel		&destChan	= serv.findChan(mess.args[0], foundChan);
	std::string broadcastMess;

	if (!checkChannelAccess(foundChan, client, mess, serv, destCli, destChan, foundCli))
		return (false);
	broadCastKick(broadcastMess, client, destCli, destChan, mess, serv);
		destChan.removeClient(&destCli);
		std::cout << GREEN << client.getNick() << " kicked " << destCli.getNick() << " from " << destChan.getName() << RESET << std::endl;
		if (destChan.getMembers().size() == 0)
			serv.deleteChan(mess.args[0]);
	return (true);
}

bool	inviteCmd(Client &client,iRCMessage &mess,Server &serv)
{
	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 "* INVITE :Not enough parameters"), serv);
		return (false);
	}
	std::string	invitationMess;
	std::string	confirmationMess;
	bool		foundCli 	= false;
	bool		foundChan 	= false;
	Client 		&invitedCli = serv.findTrueClient(mess.args[0], foundCli);
	Client 		&senderCli  = client;
	Channel 	&destChan 	= serv.findChan(mess.args[1], foundChan);

	if (!validateInvite(foundCli, client, foundChan, senderCli,
		 mess, destChan, serv, invitedCli))
		return (false);
	invitationMess += ":" + client.getNick() + "!" + client.getUser() + "@" + CurrentHostname(client);
	invitationMess += " INVITE ";
	invitationMess += invitedCli.getNick() + " ";
	invitationMess += destChan.getName();
	invitationMess += CRLF;
	confirmationMess += senderCli.getNick() + " ";
	confirmationMess += invitedCli.getNick() + " ";
	confirmationMess += destChan.getName();
	invitedCli.ConcatenateWBuffer(invitationMess, serv);
	senderCli.ConcatenateWBuffer(FormatedMessage("341", ":" + serv.getName(),
		 confirmationMess), serv);
		destChan.addInvited(&invitedCli);
		std::cout << GREEN << client.getNick() << " invited " << invitedCli.getNick() << " to " << destChan.getName() << RESET << std::endl;
		return (true);
}

bool	modeCmd(Client &client,iRCMessage &mess,Server &serv)
{
	if (mess.args.size() < 1)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 client.getNick() + " MODE :Not enough parameters"), serv);
		return (false);
	}


	bool						foundChan	= false;
	Channel 					&destChan	= serv.findChan(mess.args[0], foundChan);
	ModeAction 					act;
	ChanModes					mode;
	std::vector<std::string>	args 		= mess.args;
	std::vector<std::string>	changesToDo;
	std::string					finalFlags;
	std::string					finalArgs;
	char						currentSign = '\0';
	std::vector<std::string>	argList;

	if (!validateChannelModeAccess(serv, client, mess))
		return (false);
	changesToDo = getChangesToDo(args);
	argList = getArgList(args);
	processFlags(changesToDo, argList, mode, act, destChan, client,
		 currentSign, finalFlags, finalArgs, serv);
	std::string finalModeStr = finalFlags;
	if (!finalArgs.empty())
		finalModeStr += " " + finalArgs;
	if (!finalModeStr.empty())
		broacastModeChange(client, destChan, finalModeStr, serv);
	return (true);
}

bool	quitCmd(iRCMessage& mess, Client& client, Server& serv)
{
	std::set<std::string>	joinedChannel;
	std::string				msg;
	char					hostname[INET_ADDRSTRLEN];
	struct sockaddr_in		tmp = client.getClientInfos();

	inet_ntop(AF_INET, &tmp.sin_addr, hostname, INET_ADDRSTRLEN);
	msg += ":" + client.getNick() + "!" + client.getUser() + "@" + hostname;
	msg += " QUIT ";
	if (mess.args.empty())
	{
		mess.args.push_back("leaving");
	}
	msg += mess.args[0];
	msg += CRLF;
	joinedChannel = client.getJoinedChannels();
	for (std::set<std::string>::iterator it = joinedChannel.begin();
			it != joinedChannel.end(); it++)
	{
		bool success;
		Channel& chan = serv.findChan(*it, success);
		if (success)
		{
			serv.broadcast(msg, client, chan, serv);
			chan.removeClient(&client);
		}
		else
			return (false);
	}

	// needs to cleanUp (modify delete stuff to by fd to be more efficient)
	CleanUp(serv, client.getFd());
	std::cout << GREEN  << "Bye " << client.getNick() << RESET << std::endl;
	return (true);
}
