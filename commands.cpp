/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/10 15:31:56 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

bool	passCmd(Client &client, iRCMessage &mess, Server &serv)
{
	// std::cout << "inpasscmd" <<std::endl;
    if (client.isRegistered())
    {
		sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		return (false);
	}
    if (mess.args.empty())
    {
		sendCodes(client.getFd(), "461", ":server", "* PASS :Not enough parameters");
		return (false);
	}
    if (mess.args[0] != serv.getPass())
    {
		sendCodes(client.getFd(), "464", ":server", ":Password incorrect");
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
		sendCodes(client.getFd(), "431", ":server", ":No nickname specified");
		return (false);
	}
	newNick = mess.args[0];


	/*---------------------------------fix this ----------------------------------------------*/
	for (unsigned int i = 0; i < serv.getAllClients().size(); i++)
	{
		if (serv.getAllClients()[i].getNick() == newNick)
		{
			if (!client.isRegistered())
				sendCodes(client.getFd(), "433", ":server", "* " + newNick + " :Nickname is already in use");
			else
				sendCodes(client.getFd(), "433", ":server", client.getNick() + " " + newNick + " :Nickname is already in use");
			return (false);
		}
	}
	/*-------------------------------------------------------------------------------*/
	std::cout <<"new nick = " << newNick << std::endl;
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client);
	return (true);
}

bool	userCmd(Client &client, iRCMessage &mess)
{
	if (client.isRegistered())
    {
		sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		return (false);
	}
    if (mess.args.size() < 4)
    {
		sendCodes(client.getFd(), "461" , ":server", "* USER :Not enough parameters");
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
    send(client.getFd(), ":server CAP * LS :\r\n", 21, 0);
    send(client.getFd(), ":server CAP * END\r\n", 20, 0);
    return (true);
}

bool	pongCmd(Client &client, iRCMessage &mess)
{
	std::string pongstr;

	if (mess.args.empty())
		pongstr = CRLF;
	else
		pongstr = mess.args[0] + CRLF;
	send(client.getFd(), pongstr.c_str(), pongstr.size(), 0);
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
		sendCodes(client.getFd(), "401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick/channel");
		return (false);
	}
	if (foundClient)
	{
		messageOutput = formMess(sender, destCli, mess);
		send(destCli.getFd(), messageOutput.c_str(), messageOutput.size(), 0);
	}
	else
	{
		std::set<Client *> members = destChan.getMembers();
		for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
		{
			messageOutput = formMess(sender, **it, mess);
			send((**it).getFd(), messageOutput.c_str(), messageOutput.size(), 0);
		}
	}
	std::cout <<messageOutput<< std::endl;
	return (true);
}

bool	joinCmd(Client &client, iRCMessage &mess, Server &serv)
{
	if (!chanExists(mess.args[0], serv))
	{
		serv.getAllChans().push_back(Channel(mess.args[0], ""));
	}
	//chck channel restrictions
	//if allowed
	//1-adduser to channel;
	//2-if 1st user make user operator
	//3-broadcast join to channel
	//4-send channel stae (topic+userlist)
}
