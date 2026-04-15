/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:40 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/14 19:07:41 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <sstream>
# include <vector>
# include <string>
# include <algorithm>

//commands number
# ifndef COM_NUM
#  define COM_NUM 14
# endif

# ifndef	CRLF
# define CRLF "\r\n"
# endif




// CAP — negotiates client/server capabilities before full registration
// PASS — provides server password for authentication
// NICK — sets or changes the client’s nickname
// USER — supplies username and real name to complete registration
// PING — checks connection liveness (expects PONG reply)
// KICK — forcibly removes a user from a channel
// TOPIC — sets or retrieves the channel’s topic
// MODE — changes or queries user/channel modes (permissions and flags)
// JOIN — makes a user enter a channel (creates it if needed)
// PART — makes a user leave a channel
// PRIVMSG — sends a message to a user or channel
// INVITE — invites a user to join a channel
// QUIT — disconnects a client from the server with a message
// DISCONNECT — non-standard command to forcibly close a connection (server-specific)
typedef enum	command
{
	CAP     ,
	PASS	,
	NICK	,
	USER	,
	PING	,
	KICK	,
	TOPIC	,
	MODE	,
	JOIN	,
	PART	,
	PRIVMSG	,
	INVITE	,
	QUIT	,
	DISCONNECT,
	UNKNOWN
}				command;


struct iRCMessage
{
	std::string 				prefix;
	command						cmd;
	std::vector<std::string>	args;
	std::string 				crlf;
	size_t						len;
	std::string					ogMess;
};


void						initialiseIRCMessage(iRCMessage &msg);
iRCMessage					parseMessage(const std::string &strMess);
bool						isMessValid(const iRCMessage &mess);
std::vector<std::string>	split(const std::string& str, char delimiter);
std::vector<std::string>	splitCRLF(const std::string& str);
void						createTypeTab(std::vector<std::string>	&types);
void						assignCommand(std::vector<std::string>	&t, std::string	&cmd, iRCMessage &msg);
void						handleTrailingArgs(iRCMessage &parsedMess,
							const std::vector<std::string>	&splitMess, bool &trailing, size_t	&i);



#endif
