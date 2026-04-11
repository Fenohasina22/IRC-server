/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:56:54 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 09:52:47 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCH_HPP
# define DISPATCH_HPP
# include <iostream>
# include "parser.hpp"
# include "commands.hpp"
# include "Server.hpp"
# ifndef	CRLF
# define CRLF "\r\n"
# endif

class Server ;


void	dispatchCommand(iRCMessage &mess, Client &client, Server &serv);
void	sendCodes(const int &fd, std::string code, const std::string &prefix, const::std::string &msg);
std::string	FormatedMessage(std::string code, const std::string &prefix, const::std::string &msg);

#endif
