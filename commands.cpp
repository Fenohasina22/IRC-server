/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/13 09:55:53 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

bool	passCmd(Client &client, iRCMessage &mess, Server &serv)
{
	// std::cout << "inpasscmd" <<std::endl;
    if (client.isRegistered())
    {
		//sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		client.ConcatenateWBuffer(FormatedMessage("462", ":server", ":You may not reregister"), serv);
		return (false);
	}
    if (mess.args.empty())
    {
		//sendCodes(client.getFd(), "461", ":server", "* PASS :Not enough parameters"), serv;
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* PASS :Not enough parameters"), serv);

		return (false);
	}
    if (mess.args[0] != serv.getPass())
    {
		//sendCodes(client.getFd(), "464", ":server", ":Password incorrect"), serv;
		client.ConcatenateWBuffer(FormatedMessage("464", ":server", ":Password incorrect"), serv);

		return (false);
	}
    client.setPassState(true);
	// std::cout << "PASS ok" << std::endl;
	return (true);
}

bool	nickCmd(Client &client, iRCMessage &mess, Server &serv)
{
	std::string newNick;

	if (mess.args.empty())
    {
		//sendCodes(client.getFd(), "431", ":server", ":No nickname specified");
		client.ConcatenateWBuffer(FormatedMessage("431", ":server", ":No nickname specified"), serv);
		return (false);
	}
	newNick = mess.args[0];


	/*---------------------------------fix this ----------------------------------------------*/
	for (unsigned int i = 0; i < serv.getAllClients().size(); i++)
	{
		if (serv.getAllClients()[i].getNick() == newNick)
		{
			if (!client.isRegistered())
			{
				//sendCodes(client.getFd(), "433", ":server", "* " + newNick + " :Nickname is already in use"), serv;
				client.ConcatenateWBuffer(FormatedMessage("433", ":server", "* " + newNick + " :Nickname is already in use"), serv);
			}
			else
			{
				//sendCodes(client.getFd(), "433", ":server", client.getNick() + " " + newNick + " :Nickname is already in use"), serv;
				client.ConcatenateWBuffer(FormatedMessage("433", ":server", client.getNick() + " " + newNick + " :Nickname is already in use"), serv);
			}
			return (false);
		}
	}
	/*-------------------------------------------------------------------------------*/
	std::cout <<"new nick = " << newNick << std::endl;
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client, serv); // concatenate
	return (true);
}

bool	userCmd(Client &client, iRCMessage &mess, Server& serv)
{
	if (client.isRegistered())
    {
		//sendCodes(client.getFd(), "462", ":server", ":You may not reregister"), serv;
		client.ConcatenateWBuffer(FormatedMessage("462", ":server", ":You may not reregister"), serv);
		return (false);
	}
    if (mess.args.size() < 4)
    {
		//sendCodes(client.getFd(), "461" , ":server", "* USER :Not enough parameters"), serv;
		printiRCMESS(mess);
		client.ConcatenateWBuffer(FormatedMessage("461" , ":server", "* USER :Not enough parameters"), serv);

		return (false);
	}
    client.setUser(mess.args[0]);
    client.setReal(mess.args[3]);
    client.setUserState(true);
    tryRegistration(client, serv);
	return (true);
}

bool	capCmd(Client &client, Server& serv)
{
	if (client.isRegistered())
		return (false);
    //send(client.getFd(), ":server CAP * LS :\r\n", 21, 0);
    //send(client.getFd(), ":server CAP * END\r\n", 20, 0);
	std::string		cap;

	client.ConcatenateWBuffer(":server CAP * LS :\r\n", serv);
	if (client.isRegistered())
		client.ConcatenateWBuffer(":server CAP * END\r\n", serv);
    std::cout << GREEN << "U got CAP HERE" << RESET << std::endl;

	return (true);
}

bool	pongCmd(Client &client, iRCMessage &mess, Server& serv)
{
	std::string pongstr;

	if (mess.args.empty())
		pongstr = CRLF;
	else
		pongstr = "PONG " + mess.args[0] + CRLF;
	//send(client.getFd(), pongstr.c_str(), pongstr.size(), 0);
	client.ConcatenateWBuffer(pongstr, serv);
	return (true);
}

bool	privmsgCmd(Client &client, iRCMessage &mess, Server &serv)
{
	bool			foundClient 	= false;
	bool			foundChan	 	= false;
	Client			&destCli	 	= serv.findClient(mess.args[0], foundClient);
	Channel			&destChan		= serv.findChan(mess.args[0], foundChan);
	const Client 	&sender 		= client;
	std::string 	messageOutput;

	if (!foundClient && !foundChan)
	{
		//sendCodes(client.getFd(), "401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick/channel");
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick/channel"), serv);
		return (false);
	}
	if (foundClient)
	{
		messageOutput = formMess(sender, destCli, mess);
		//send(destCli.getFd(), messageOutput.c_str(), messageOutput.size(), 0);
		destCli.ConcatenateWBuffer(messageOutput, serv);
	}
	else
	{
		std::set<std::string> members = destChan.getMembers();
		for (std::set<std::string>::iterator it = members.begin(); it != members.end(); ++it)
		{
			bool found = false;
			Client &target = serv.findClient(*it, found);
			if (!found)
				continue;
			messageOutput = formChanMess(sender, destChan, mess);
			if (target.getNick() != client.getNick())
				target.ConcatenateWBuffer(messageOutput, serv);
				//send(target.getFd(), messageOutput.c_str(), messageOutput.size(), 0);

		}
	}
	std::cout <<messageOutput<< std::endl;
	return (true);
}

//add these:
// 473 nick #channel :Cannot join channel (+i)
// 471 nick #channel :Cannot join channel (+l)
// 475 nick #channel :Cannot join channel (+k)
// 474 nick #channel :Cannot join channel (+b)
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
	//if allowed
	//1-adduser to channel;
	if (client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("443", ":server", client.getNick() + " " + mess.args[0] + " :is already on channel"), serv);
		return (false);
	}
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
		if (!client.isInChannel(destChan.getName()))
		{
			client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
			return (false);
		}
		// else if (no permision) -> error code + do nothing
	}
	else if (resetTopic)
	{
		if (!client.isInChannel(destChan.getName()))
		{
			client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
			return (false);
		}
		// else if (no permision) -> error code + do nothing
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
	Client	&destCli = serv.findClient(mess.args[1], foundCli);
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
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* INVITE :Not enough parameters"), serv);

	bool	foundCli 	= false;
	bool	foundChan 	= false;
	Client 	&invitedCli = serv.findClient(mess.args[0], foundCli);
	Client 	&senderCli  = client;
	Channel &destChan 	= serv.findChan(mess.args[1], foundChan);

	if (!foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick"), serv);
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
}
