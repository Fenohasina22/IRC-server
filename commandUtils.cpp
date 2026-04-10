/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:10:46 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/10 15:17:58 by mratsima         ###   ########.fr       */
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
		std::string msg001 = ":server 001 " + client.getNick() + " :Welcome to the IRC " + client.getNick() + CRLF;
		std::string msg002 = ":server 002 " + client.getNick() + " :Your host is server" + CRLF;
		std::string msg003 = ":server 003 " + client.getNick() + " :This server was created just now" + CRLF;
		std::string msg004 = ":server 004 " + client.getNick() + " idk bro put all the infos abt our server" + CRLF;
		// std::string all = msg001 + msg002 + msg003 + msg004;
		send(client.getFd(), CRLF , 2, 0);
		send(client.getFd(), msg001.c_str(), msg001.size(), 0);
		send(client.getFd(), msg002.c_str(), msg002.size(), 0);
		send(client.getFd(), msg003.c_str(), msg003.size(), 0);
		send(client.getFd(), msg004.c_str(), msg004.size(), 0);
		// send(client.getFd(), all.c_str(), all.size(), 0);
	}
}

std::string	formMess(const Client	&sender,const Client &destCli,const iRCMessage &mess)
{
	std::string 	messageOutput;

	messageOutput += ":" + sender.getNick();
	messageOutput += " PRIVMSG ";
	messageOutput += destCli.getNick() + " ";
	messageOutput += mess.args[1];
	messageOutput += CRLF;
	return (messageOutput);
}

bool 	chanExists(const std::string &chanName, Server &serv)
{
	bool	foundChan;

	serv.findChan(chanName, foundChan);
	if (foundChan)
		return (true);
	return (false);
}
