/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 17:53:36 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

bool	passCmd(Client &client, iRCMessage &mess, Server &serv)
{
	// std::cout << "inpasscmd" <<std::endl;
    if (client.isRegistered())
    {
		//sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		client.ConcatenateWBuffer(FormatedMessage("462", ":server", ":You may not reregister"));
		return (false);
	}
    if (mess.args.empty())
    {
		//sendCodes(client.getFd(), "461", ":server", "* PASS :Not enough parameters");
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* PASS :Not enough parameters"));

		return (false);
	}
    if (mess.args[0] != serv.getPass())
    {
		//sendCodes(client.getFd(), "464", ":server", ":Password incorrect");
		client.ConcatenateWBuffer(FormatedMessage("464", ":server", ":Password incorrect"));
		
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
		client.ConcatenateWBuffer(FormatedMessage("431", ":server", ":No nickname specified"));
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
				//sendCodes(client.getFd(), "433", ":server", "* " + newNick + " :Nickname is already in use");
				client.ConcatenateWBuffer(FormatedMessage("433", ":server", "* " + newNick + " :Nickname is already in use"));
			}
			else
			{
				//sendCodes(client.getFd(), "433", ":server", client.getNick() + " " + newNick + " :Nickname is already in use");
				client.ConcatenateWBuffer(FormatedMessage("433", ":server", client.getNick() + " " + newNick + " :Nickname is already in use"));
			}
			return (false);
		}
	}
	/*-------------------------------------------------------------------------------*/
	std::cout <<"new nick = " << newNick << std::endl;
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client); // concatenate
	return (true);
}

bool	userCmd(Client &client, iRCMessage &mess)
{
	if (client.isRegistered())
    {
		//sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		client.ConcatenateWBuffer(FormatedMessage("462", ":server", ":You may not reregister"));
		return (false);
	}
    if (mess.args.size() < 4)
    {
		//sendCodes(client.getFd(), "461" , ":server", "* USER :Not enough parameters");
		printiRCMESS(mess);
		client.ConcatenateWBuffer(FormatedMessage("461" , ":server", "* USER :Not enough parameters"));
		
		return (false);
	}
    client.setUser(mess.args[0]);
    client.setReal(mess.args[3]);
    client.setUserState(true);
    tryRegistration(client);
	return (true);
}

bool	capCmd(Client &client)
{
	if (client.isRegistered())
		return (false);
    //send(client.getFd(), ":server CAP * LS :\r\n", 21, 0);
    //send(client.getFd(), ":server CAP * END\r\n", 20, 0);
	std::string		cap;

	client.ConcatenateWBuffer(":server CAP * LS :\r\n");
	if (client.isRegistered())
		client.ConcatenateWBuffer(":server CAP * END\r\n");
    std::cout << GREEN << "U got CAP HERE" << RESET << std::endl;

	return (true);
}

bool	pongCmd(Client &client, iRCMessage &mess)
{
	std::string pongstr;

	if (mess.args.empty())
		pongstr = CRLF;
	else
		pongstr = mess.args[0] + CRLF;
	//send(client.getFd(), pongstr.c_str(), pongstr.size(), 0);
	client.ConcatenateWBuffer(pongstr);
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
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick/channel"));
		return (false);
	}
	if (foundClient)
	{
		messageOutput = formMess(sender, destCli, mess);
		//send(destCli.getFd(), messageOutput.c_str(), messageOutput.size(), 0);
		client.ConcatenateWBuffer(messageOutput);
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
			messageOutput = formMess(sender, target, mess);
			if (target.getNick() != client.getNick())
				client.ConcatenateWBuffer(messageOutput);
				//send(target.getFd(), messageOutput.c_str(), messageOutput.size(), 0);

		}
	}
	std::cout <<messageOutput<< std::endl;
	return (true);
}

//add these:
// 403 nick #channel :No such channel
// 473 nick #channel :Cannot join channel (+i)
// 471 nick #channel :Cannot join channel (+l)
// 475 nick #channel :Cannot join channel (+k)
// 474 nick #channel :Cannot join channel (+b)
// 443 nick #channel :is already on channel ??? does irssi need this?
bool	joinCmd(Client &client, iRCMessage &mess, Server &serv)
{
	Channel 	*destChan;
	bool		foundChan = false;
	std::string broadcastMess;

	if (!chanExists(mess.args[0], serv))
	{
		serv.getAllChans().push_back(Channel(mess.args[0], ""));
		destChan = &(serv.getAllChans().back());
	}
	else
		destChan = &(serv.findChan(mess.args[0], foundChan));
	//chck channel restrictions
	//if allowed
	//1-adduser to channel;
	if (client.isInChannel(mess.args[0]))
		return (false);
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
	send(client.getFd(), broadcastMess.c_str(), broadcastMess.size(), 0);
	serv.broadcast(broadcastMess, client, *destChan);
	//4-send channel stae (topic+userlist)
	sendChannelState(client, *destChan);
	return (true);
}
