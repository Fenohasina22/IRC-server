/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:49:06 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/08 15:36:54 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include <iostream>
# include "parser.hpp"
# include "../Client.hpp"
# include <sys/socket.h>
# include "../Server.hpp"

# ifndef	CRLN
# define CRLN "\r\n"
# endif

class Server;

bool	capCmd(Client &client);
bool	passCmd(Client &client, iRCMessage &mess, Server &serv);
bool	nickCmd(Client &client, iRCMessage &mess, Server &serv);
bool	userCmd(Client &client, iRCMessage &mess);
bool	PongCmd(Client &client, iRCMessage &mess);

#endif
