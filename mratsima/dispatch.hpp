/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:56:54 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 21:34:00 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCH_HPP
# define DISPATCH_HPP
# include <iostream>
# include "parser.hpp"
# include "commands.hpp"
# include "../Server.hpp"
class Server ;

bool	passCmd(Client &client, iRCMessage &mess, Server &serv);
bool	nickCmd(Client &client, iRCMessage &mess, Server &serv);
bool	userCmd(Client &client, iRCMessage &mess);
void	dispatchCommand(iRCMessage &mess, Client &client, Server &serv);

#endif
