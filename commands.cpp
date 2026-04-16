/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/16 14:22:39 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

bool	passCmd(Client &client, iRCMessage &mess, Server &serv, bool &validPass)
{
    if (client.isRegistered())
    {
		client.ConcatenateWBuffer(FormatedMessage("462", ":server",
			 "* :You may not reregister"), serv);
		return (false);
	}
	if (mess.args.empty())
    {
		std::cout << RED << mess.ogMess << RESET << std::endl;
		client.ConcatenateWBuffer(FormatedMessage("461", ":server",
			 "* PASS :Not enough parameters"), serv);
		return (false);
	}
	if (mess.args[0] != serv.getPass())
    {
		client.ConcatenateWBuffer(FormatedMessage("464", ":server",
			 "* :Password incorrect"), serv);
		DeleteVecElement(serv.getVecPoll(), client.getFd());
		DeleteVecElementClient(serv.getAllClients(), client.getFd());
		close (client.getFd());
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

	if (mess.args.empty())
    {
		client.ConcatenateWBuffer(FormatedMessage("431", ":server",
			 "* :No nickname specified"), serv);
		return (false);
	}
	newNick = mess.args[0];
	if (!newNick.empty() && newNick[0] == ':')
		newNick = newNick.substr(1);
	if (isNicknameInUse(serv, client, newNick))
		return (false);
	if (!notifyNeighbors(client, serv, newNick))
		return (false);
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client, serv);
	return (true);
}

bool	userCmd(Client &client, iRCMessage &mess, Server& serv)
{
	std::string newUser;

	if (client.isRegistered())
    {
		client.ConcatenateWBuffer(FormatedMessage("462", ":server",
			 "* :You may not reregister"), serv);
		return (false);
	}
	if (mess.args.size() < 4)
    {
		client.ConcatenateWBuffer(FormatedMessage("461" , ":server",
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
		client.ConcatenateWBuffer(":server CAP * LS :\r\n", serv);
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
	return (true);
}

bool	privmsgCmd(Client &client, iRCMessage &mess, Server &serv)
{
	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server",
			 "* PRIVMSG :Not enough parameters"), serv);
		return (false);
	}

	bool			foundClient 	= false;
	bool			foundChan	 	= false;
	Client			&destCli	 	= serv.findTrueClient(mess.args[0], foundClient);
	Channel			&destChan		= serv.findChan(mess.args[0], foundChan);
	Client 			&sender 		= client;
	std::string 	messageOutput;

	if (!foundClient && !foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":server",
			 client.getNick() + " " + mess.args[0] + " :No such nick/channel"), serv);
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
	return (true);
}

bool	joinCmd(Client &client, iRCMessage &mess, Server &serv)
{
	Channel 	*destChan;
	bool		foundChan = false;
	std::string broadcastMess;

	if (mess.args.empty())
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* JOIN :Not enough parameters"), serv);
		return (false);
	}
	if (!chanExists(mess.args[0], serv))
	{
		if (mess.args[0][0] != '#')
		{
			client.ConcatenateWBuffer(FormatedMessage("403", ":server", client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
			return (false);
		}
		serv.getAllChans().push_back(Channel(mess.args[0], ""));
		destChan = &(serv.getAllChans().back());
	}
	else
		destChan = &(serv.findChan(mess.args[0], foundChan));
	//chck channel restrictions
	if (client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("443", ":server", client.getNick() + " " + mess.args[0] + " :is already on channel"), serv);
		return (false);
	}
	if (destChan->isInviteOnly() && !destChan->isInvited(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("473", ":server", client.getNick() + " " + destChan->getName() + " :cannot join channel (+i)"), serv);
		return (false);
	}
	if (destChan->isPassRequired() && (mess.args.size() < 2 || mess.args[1] != destChan->getPass()))
	{
		client.ConcatenateWBuffer(FormatedMessage("475", ":server", client.getNick() + " " + destChan->getName() + " :cannot join channel (+k)"), serv);
		return (false);
	}
	if (static_cast<int>(destChan->getMembers().size() + 1) > destChan->getMaxUser())
	{
		client.ConcatenateWBuffer(FormatedMessage("471", ":server", client.getNick() + " " + destChan->getName() + " :cannot join channel (+l)"), serv);
		return (false);
	}
	//if allowed
	//1-adduser to channel;
	destChan->addClient(&client);
	//2-if 1st user make user operator
	if (destChan->getMembers().size() == 1)
		destChan->addOperator(&client);
	//3-broadcast join to channel
	// :nick!user@host JOIN #channel
	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
	broadcastMess += " JOIN ";
	broadcastMess += destChan->getName();
	broadcastMess += CRLF;
	// send(client.getFd(), broadcastMess.c_str(), broadcastMess.size(), 0);
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, *destChan, serv);
	if (destChan->isInvited(client.getNick()))
		destChan->removeInvited(&client);
	//4-send channel state (topic+userlist)
	sendChannelState(client, *destChan, serv);
	return (true);
}

bool	partCmd(Client &client, iRCMessage &mess, Server &serv)
{
	std::string broadcastMess;
	bool		foundChan;

	if (mess.args.empty())
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* PART :Not enough parameters"), serv);
		return (false);
	}
	// Check:
	// channel exists
	if (!chanExists(mess.args[0], serv))
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":server", client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	// user is in it
	if (!client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
		return (false);
	}
	// If valid:
	// broadcast PART to channel
	Channel &destChan = serv.findChan(mess.args[0], foundChan);

	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
	broadcastMess += " PART ";
	broadcastMess += destChan.getName();
	if (mess.args.size() > 1)
		broadcastMess += mess.args[1];
	broadcastMess += CRLF;
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, destChan, serv);
	// remove user from channel
	destChan.removeClient(&client);
	if (destChan.getMembers().size() == 0)
		serv.deleteChan(mess.args[0]);
	return (true);
}

//add permission chek
bool	topicCmd(Client &client, iRCMessage &mess, Server &serv)
{
	bool		displayTopic	= false;
	bool		changeTopic		= false;
	bool		resetTopic		= false;
	bool 		foundChan		= false;
	std::string	broadcastMess;

	if (mess.args.empty())
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* TOPIC :Not enough parameters"), serv);
		return (false);
	}
	else if (mess.args.size() == 1)
		displayTopic = true;
	else if (mess.args.size() > 1 && mess.args[1] == ":")
		resetTopic = true;
	else if (mess.args.size() > 1)
		changeTopic = true;

	Channel &destChan = serv.findChan(mess.args[0], foundChan);

	if (!foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":server", client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (displayTopic)
	{
		if (destChan.getTopic() == "")
		{
			client.ConcatenateWBuffer(FormatedMessage("331", ":server", client.getNick() + " " + destChan.getName() + " :No topic is set"), serv);
			return (false);
		}
		client.ConcatenateWBuffer(FormatedMessage("332", ":server", client.getNick() + " " + destChan.getName() + " :" + destChan.getTopic()), serv);
	}
	else if (changeTopic)
	{
		if (destChan.isTopicLocked() && !destChan.isOps(client.getNick()))
		{
			client.ConcatenateWBuffer(FormatedMessage("482", ":server", client.getNick() + " " + destChan.getName() + " :You're not a channel operator"), serv);
			return (false);
		}
		if (!client.isInChannel(destChan.getName()))
		{
			client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
			return (false);
		}
	}
	else if (resetTopic)
	{
		if (destChan.isTopicLocked() && !destChan.isOps(client.getNick()))
		{
			client.ConcatenateWBuffer(FormatedMessage("482", ":server", client.getNick() + " " + destChan.getName() + " :You're not a channel operator"), serv);
			return (false);
		}
		if (!client.isInChannel(destChan.getName()))
		{
			client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
			return (false);
		}
	}
	if (changeTopic || resetTopic)
	{
		std::string newTopic;
		if (resetTopic)
			newTopic = "";
		else
			newTopic = mess.args[1];
		if (!newTopic.empty() && newTopic[0] == ':')
			newTopic = newTopic.substr(1, newTopic.size());
		destChan.setTopic(newTopic);
		broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
		broadcastMess += " TOPIC ";
		broadcastMess += destChan.getName();
		broadcastMess += " :" + destChan.getTopic();
		broadcastMess += CRLF;
		client.ConcatenateWBuffer(broadcastMess, serv);
		serv.broadcast(broadcastMess, client, destChan, serv);
	}
	return (true);
}

bool	kickCmd(Client &client,iRCMessage &mess,Server &serv)
{
	// :nick!user@host KICK #channel target :reason
	std::string broadcastMess;

	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* KICK :Not enough parameters"), serv);
		return (false);
	}
	if (!chanExists(mess.args[0], serv))
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":server", client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}

	bool	foundCli = false;
	bool	foundChan = false;
	Client	&destCli = serv.findTrueClient(mess.args[1], foundCli);
	Channel	&destChan = serv.findChan(mess.args[0], foundChan);

	if (!foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":server", client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (!client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
		return (false);
	}
	if (!foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + mess.args[1] + " :No such nick/channel"), serv);
		return (false);
	}
	if (!destCli.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("441", ":server", destCli.getNick() + " " + mess.args[0] + " :They aren't on that channel"), serv);
		return (false);
	}
	if (!destChan.isOps(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":server", client.getNick() + " " + mess.args[0] + " :You're not a channel operator"), serv);
		return (false);
	}

	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
	broadcastMess += " KICK ";
	broadcastMess += destChan.getName() + " ";
	broadcastMess += destCli.getNick();
	if (mess.args.size() > 2)
	{
		std::string reason = mess.args[2];
		if (!reason.empty() && reason[0] == ':')
			reason = reason.substr(1);
		broadcastMess += " :" + reason;
	}
	broadcastMess += CRLF;
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, destChan, serv);
	destChan.removeClient(&destCli);
	if (destChan.getMembers().size() == 0)
		serv.deleteChan(mess.args[0]);
	return (true);
}

bool	inviteCmd(Client &client,iRCMessage &mess,Server &serv)
{
	// Usage: INVITE nick #channel
	std::string	invitationMess;
	std::string	confirmationMess;

	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* INVITE :Not enough parameters"), serv);
		return (false);
	}

	bool	foundCli 	= false;
	bool	foundChan 	= false;
	Client 	&invitedCli = serv.findTrueClient(mess.args[0], foundCli);
	Client 	&senderCli  = client;
	Channel &destChan 	= serv.findChan(mess.args[1], foundChan);

	if (!foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick"), serv);
		return (false);
	}
	if (!foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":server", destChan.getName() + " " + mess.args[1] + " :No such channel"), serv);
		return (false);
	}
	if (!senderCli.isInChannel(mess.args[1]))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + destChan.getName() + " :You're not on that channel"), serv);
		return (false);
	}
	if (!destChan.isOps(senderCli.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":server", client.getNick() + " " + destChan.getName() + " :You're not a channel operator"), serv);
		return (false);
	}
	if (invitedCli.isInChannel(destChan.getName()))
	{
		client.ConcatenateWBuffer(FormatedMessage("443", ":server", client.getNick() + " " + destChan.getName() + " :is already on channel"), serv);
		return (false);
	}
	//to the invited ---> :nick!user@host INVITE target #channel
	//to the sender ---> :server 341 sender target #channel

	invitationMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
	invitationMess += " INVITE ";
	invitationMess += invitedCli.getNick() + " ";
	invitationMess += destChan.getName();
	invitationMess += CRLF;

	confirmationMess += senderCli.getNick() + " ";
	confirmationMess += invitedCli.getNick() + " ";
	confirmationMess += destChan.getName();

	invitedCli.ConcatenateWBuffer(invitationMess, serv);
	senderCli.ConcatenateWBuffer(FormatedMessage("341", ":server", confirmationMess), serv);

	destChan.addInvited(&invitedCli);
	return (true);
}

bool	modeCmd(Client &client,iRCMessage &mess,Server &serv)
{
	//MODE #channel chanmode

	if (mess.args.size() < 1)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", client.getNick() + " MODE :Not enough parameters"), serv);
		return (false);
	}


	bool	foundChan = false;
	bool	foundCli = false;
	Channel &destChan = serv.findChan(mess.args[0], foundChan);
	serv.findTrueClient(mess.args[0], foundCli);
	if (!foundChan && !foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":server", client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (foundCli)
		return (false);
	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("324", ":server", client.getNick() + " " + destChan.getName() + " " + destChan.flagsToStr()), serv);
		return (true);
	}
	if (!destChan.isOps(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":server", client.getNick() + " " + mess.args[0] + " :You're not a channel operator"), serv);
		return (false);
	}

	ModeAction 					act;
	ChanModes					mode;
	std::vector<std::string>	args = mess.args;

	mode = strToMode(mess.args[1], act);
	if (mode == unknown || act == NO_ACTION)
	{
		// 472 nick x :is unknown mode char to me
		client.ConcatenateWBuffer(FormatedMessage("472", ":server", client.getNick() + " " + mess.args[1] + " :is unknown mode char to me"), serv);
		return (false);
	}
	//server replies --> 324 nick #chan +modes
	switch (mode)
	{
		case i:
			doIflag(destChan, act);
			break;

		case t:
			doTflag(destChan, act);
			break;

		case k:
			if (!doKflag(destChan, act, mess.args))
			{
				client.ConcatenateWBuffer(FormatedMessage("461", ":server", client.getNick() + " MODE :Not enough parameters"), serv);
				return (false);
			}
			break;

		case l:
			if (!doLflag(destChan, act, mess.args))
			{
				client.ConcatenateWBuffer(FormatedMessage("461", ":server", client.getNick() + " MODE :Not enough parameters"), serv);
				return (false);
			}
			break;
		case o:
			if (doOflag(destChan, act, mess.args, client, serv))
				return (false);
		default:
			break;
	}
	std::string broadcastMess;

	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
	broadcastMess += " MODE ";
	broadcastMess += destChan.getName() + " ";
	broadcastMess += mess.args[1];
	broadcastMess += CRLF;

	serv.broadcast(broadcastMess, client, destChan, serv);
	client.ConcatenateWBuffer(broadcastMess, serv);
	return (true);
}

bool	quitCmd(iRCMessage& mess, Client& client, Server& serv)
{
	std::set<std::string>	joinedChannel;
	std::string				msg;
	char					hostname[INET_ADDRSTRLEN];
	struct sockaddr_in		tmp = client.getClientInfos();


	// transform the binary into string
	inet_ntop(AF_INET, &tmp.sin_addr, hostname, INET_ADDRSTRLEN);
	msg = ":" + client.getNick() + "!" + client.getUser() + "@" + hostname + " QUIT " + mess.args[0] + CRLF;
	joinedChannel = client.getJoinedChannels();
	for (std::set<std::string>::iterator it = joinedChannel.begin(); it != joinedChannel.end(); it++)
	{
		bool success;
		Channel& chan = serv.findChan(*it, success);
		if (success)
		{
			serv.broadcast(msg, client, chan, serv);
			chan.removeClient(&client);
		}
		else
		{
			return (false);
		}
	}
	std::cout << RED << msg << RESET << std::endl;
	std::cout << GREEN  << "Bye " << client.getUser() << RESET << std::endl;
	return (true);
}

bool	disconnectCmd(Client& client, Server& serv)
{
	(void) serv;
	std::cout << GREEN <<  client.getUser() << " disconected" << RESET << std::endl;
	return (true);
}
