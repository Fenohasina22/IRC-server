/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/08 15:55:10 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void 	tryRegistration(Client &client)
{
	if (client.isRegistered())
		return ;
	if (client.getNickState() && client.getPassState() && client.getUserState())
	{
		client.setRegistrationState(true);
		std::string msg001 = ":server 001 " + client.getNick() + " :Welcome to the IRC " + client.getNick() + CRLN;
		std::string msg002 = ":server 002 " + client.getNick() + " :Your host is server" + CRLN;
		std::string msg003 = ":server 003 " + client.getNick() + " :This server was created just now" + CRLN;
		std::string msg004 = ":server 004 " + client.getNick() + " idk bro put all the infos abt our server" + CRLN;
		// std::string all = msg001 + msg002 + msg003 + msg004;
		send(client.getFd(), CRLN , 2, 0);
		send(client.getFd(), msg001.c_str(), msg001.size(), 0);
		send(client.getFd(), msg002.c_str(), msg002.size(), 0);
		send(client.getFd(), msg003.c_str(), msg003.size(), 0);
		send(client.getFd(), msg004.c_str(), msg004.size(), 0);
		// send(client.getFd(), all.c_str(), all.size(), 0);
	}
}

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
	for (unsigned int i = 0; i < serv.getAllClients().size(); i++)
	{
		if (serv.getAllClients()[i].getNick() == newNick)
		{
			sendCodes(client.getFd(), "433", ":server", ":Nickname already taken");
			return (false);
		}
	}
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
    send(client.getFd(), ":server CAP * LS :\r\n", 21, 0);
    send(client.getFd(), ":server CAP * END\r\n", 20, 0);
    return (true);
}

bool	PongCmd(Client &client, iRCMessage &mess)
{
	std::string pongstr;

	if (mess.args.empty())
		pongstr = CRLN;
	else
		pongstr = mess.args[0] + CRLN;
	send(client.getFd(), pongstr.c_str(), pongstr.size(), 0);
	return (true);
}
