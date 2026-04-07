/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 21:17:48 by fsamy-an         ###   ########.fr       */
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
		std::string msg001 = ":server 001 " + client.getNick() + " :Welcome to mratsima&fsamy-an's IRC" + client.getNick() + "\r\n";
		std::string msg002 = ":server 002 " + client.getNick() + " :Your host is server\r\n";
		std::string msg003 = ":server 003 " + client.getNick() + " :This server was created just now\r\n";
		std::string msg004 = ":server 004 " + client.getNick() + " idk bro put all the infos abt our server\r\n";

		send(client.getFd(), msg001.c_str(), msg001.size(), 0);
		send(client.getFd(), msg002.c_str(), msg002.size(), 0);
		send(client.getFd(), msg003.c_str(), msg003.size(), 0);
		send(client.getFd(), msg004.c_str(), msg004.size(), 0);
	}
}

bool	passCmd(Client &client, const iRCMessage &mess, Server &serv)
{
    if (client.isRegistered())
    {
	    send(client.getFd(), ":server 462 * :You may not reregister\r\n", 40, 0);
		return (false);
	}
    if (mess.args.empty())
    {
	    send(client.getFd(), ":server 461 * PASS :Not enough parameters\r\n", 44, 0);
		return (false);
	}
    if (mess.args[0] != serv.getPass())
    {
	    send(client.getFd(), ":server 464 * :Password incorrect\r\n", 36, 0);
		return (false);
	}
    client.setPassState(true);
	return (true);
}

bool	nickCmd(Client &client, const iRCMessage &mess, Server &serv)
{
	std::string newNick;

	if (mess.args.empty())
    {
	    send(client.getFd(), ":server 431 :No nickname specified\r\n", 44, 0);
		return (false);
	}
	newNick = mess.args[0];
	for (unsigned int i = 0; i < serv.getAllClients().size(); i++)
	{
		if (serv.getAllClients()[i].getNick() == newNick)
		{
			send(client.getFd(), ":server 433 :Nicknaem already taken\r\n", 44, 0);
			return (false);
		}
	}
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client);
	return (true);
}

bool	userCmd(Client &client, const iRCMessage &mess)
{
	if (client.isRegistered())
    {
		send(client.getFd(), ":server 462 * :You may not reregister\r\n", 40, 0);
		return (false);
	}   
    if (mess.args.size() < 4)
    {
		send(client.getFd(), ":server 461 * USER :Not enough parameters\r\n", 44, 0);
		return (false);
	}   
    client.setUser(mess.args[0]);
    client.setReal(mess.args[3]);
    client.setUserState(true);
    tryRegistration(client);
	return (true);
}
