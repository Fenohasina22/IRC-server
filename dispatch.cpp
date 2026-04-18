/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:57:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 19:22:22 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatch.hpp"

void	dispatchCommand(iRCMessage &mess, Client &client, Server &serv, bool &validPass)
{
	if (!client.isRegistered() && mess.cmd != CAP
	&& mess.cmd != PASS && mess.cmd != NICK && mess.cmd != USER)
	{
		client.ConcatenateWBuffer(FormatedMessage("451", ":" + serv.getName(),
		 "* :You have not registered"), serv);
		return ;
	}
	if (!isMessValid(mess))
		return ;
	switch (mess.cmd)
	{
		case (CAP):
			capCmd(client, mess, serv);
			break ;
		case (PASS):
			passCmd(client, mess, serv, validPass);
			break;
		case (NICK):
			nickCmd(client, mess, serv);
			break;
		case (USER):
			userCmd(client, mess, serv);
			break;
		case (PING):
			pongCmd(client, mess, serv);
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
			quitCmd(mess ,client, serv);
			break;
		default:
			client.ConcatenateWBuffer(FormatedMessage("421", ":" + serv.getName(),
				 client.getNick() + " " + mess.strCmd + " :Unknown command"), serv);
			std::cout << RED << "UNKNOWN COMMAND" << RESET << std::endl;
			break;
	}
}

std::string	FormatedMessage(std::string code, const std::string &prefix, const std::string &msg)
{
	std::string completeMsg = prefix + " " + code + " " + msg + CRLF;
	std::cout << completeMsg << std::endl;
	return (completeMsg);
}
