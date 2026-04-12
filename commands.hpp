/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:49:06 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/12 09:56:58 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include <iostream>
# include "parser.hpp"
# include "Client.hpp"
# include <sys/socket.h>
# include "Server.hpp"

# ifndef	CRLF
# define CRLF "\r\n"
# endif

class Server;

/************************************COMMANDS*********************************/
bool	capCmd(Client &client, Server& serv);
bool	passCmd(Client &client, iRCMessage &mess, Server& serv);
bool	nickCmd(Client &client, iRCMessage &mess, Server& serv);
bool	userCmd(Client &client, iRCMessage &mess, Server& serv);
bool	pongCmd(Client &client, iRCMessage &mess, Server& serv);
bool	privmsgCmd(Client &client, iRCMessage &mess, Server &Serv);
bool	joinCmd(Client &client, iRCMessage &mess, Server &Serv);


/************************************UTILS************************************/
void 		tryRegistration(Client &client, Server& serv);
bool	 	chanExists(const std::string &chanName, Server &serv);
std::string	formMess(const Client	&sender,const Client &destCli
			,const iRCMessage &mess);
void		sendChannelState(Client &client, Channel &destChan, Server& serv);

#endif
