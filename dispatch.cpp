/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:57:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/13 11:07:27 by mratsima         ###   ########.fr       */
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
			capCmd(client, serv); // concatenated
			break ;
		case (PASS):
			passCmd(client, mess, serv); // concatenated
			break;
		case (NICK):
			nickCmd(client, mess, serv); // concatenated
			break;
		case (USER):
			userCmd(client, mess, serv); // concatenated
			break;
		case (PING):
			pongCmd(client, mess, serv); // concatenated
			break;
		case (KICK):
			kickCmd(client, mess, serv);
			break;
		case (TOPIC):
			topicCmd(client, mess, serv);
			break;
		case (MODE):
			modeCmd(client, mess, serv);
			break;
		case (JOIN):
			joinCmd(client, mess, serv);
			break;
		case (PART):
			partCmd(client, mess, serv);
			break;
		case (PRIVMSG):
			privmsgCmd(client, mess, serv);
			break;
		case (INVITE):
			inviteCmd(client, mess, serv);
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
