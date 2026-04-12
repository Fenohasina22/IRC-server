/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:57:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/12 08:20:44 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatch.hpp"

void	dispatchCommand(iRCMessage &mess, Client &client, Server &serv)
{
	//1-if !registered && !registering_command:return;
	// std::cout << "indispatch" <<std::endl;
	if (!client.isRegistered() && mess.cmd != CAP
		&& mess.cmd != PASS && mess.cmd != NICK && mess.cmd != USER)
		return ;
	// if (!isMessValid(mess))
	// 	return ;
	switch (mess.cmd)
	{
		case (CAP):
			capCmd(client); // concatenated
			break ;
		case (PASS):
			passCmd(client, mess, serv); // concatenated
			break;
		case (NICK):
			nickCmd(client, mess, serv); // concatenated
			break;
		case (USER):
			userCmd(client, mess); // concatenated
			break;
		case (PING):
			pongCmd(client, mess); // concatenated
			break;
		case (KICK):
			//KICK command handler;
			break;
		case (TOPIC):
			//TOPIC command handler
			break;
		case (MODE):
			//MODE command handler
			break;
		case (JOIN):
			joinCmd(client, mess, serv);
			break;
		case (PART):
			partCmd(client, mess, serv);
			break;
		case (PRIVMSG):
			privmsgCmd(client, mess, serv); // concatenated
			break;
		case (QUIT):
			//QUIT command handler
			break;
		default:
			//HANDLE AS UNKNOWN COMMAND 421
			std::cout << RED << "UNKNOWN COMMAND" << RESET << std::endl;
			break;
	}
}

void	sendCodes(const int &fd, std::string code, const std::string &prefix, const::std::string &msg)
{
	std::string completeMsg = prefix + " " + code + " " + msg + CRLF;
	std::cout << completeMsg << std::endl;
	send(fd, completeMsg.c_str(), completeMsg.size(), 0);
}


std::string	FormatedMessage(std::string code, const std::string &prefix, const::std::string &msg)
{
	std::string completeMsg = prefix + " " + code + " " + msg + CRLF;
	std::cout << completeMsg << std::endl;
	return (completeMsg);
}
