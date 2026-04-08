/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:57:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/08 08:23:33 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatch.hpp"

void	dispatchCommand(iRCMessage &mess, Client &client, Server &serv)
{
	//1-if !registered && !registering_command:return;
	// std::cout << "indispatch" <<std::endl;
	// if (!client.isRegistered()
	// 	&& mess.command != PASS && mess.command != NICK && mess.command != USER)
	// 	return ;
	// if (!isMessValid(mess))
	// 	return ;
	switch (mess.cmd)
	{
		case (CAP):
			capCmd(client);
			break ;
		case (PASS):
			passCmd(client, mess, serv);
			break;
		case (NICK):
			nickCmd(client, mess, serv);
			break;
		case (USER):
			userCmd(client, mess);
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
			//JOIN command handler
			break;
		case (PART):
			//PART command handler
			break;
		case (PRIVMSG):
			//PRIVMSG command handler
			break;
		case (QUIT):
			//QUIT command handler
			break;
		default:
			//HANDLE AS UNKNOWN COMMAND 421
			break;
	}
}
