/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:49:06 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 21:13:38 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include <iostream>
# include "parser.hpp"
# include "../Client.hpp"
# include <sys/socket.h>
# include "../Server.hpp"
class Server;

bool	passCmd(Client &client, iRCMessage &mess, Server &serv);
bool	nickCmd(Client &client, iRCMessage &mess, Server &serv);
bool	userCmd(Client &client, iRCMessage &mess);


#endif
