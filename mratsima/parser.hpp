/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:40 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/08 14:26:48 by mratsima         ###   ########.fr       */
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
#  define COM_NUM 11
# endif

//KICK - Eject a client from the channel
//TOPIC - Change or view the channel topic
//MODE - Change the channel’s mode:
//JOIN	Client joins a channel — created automatically if it doesn't exist. First user becomes channel operator. Send back topic + names list
//PART	Client leaves a channel
//PRIVMSG	Send message to a channel or user
//TOPIC	View or set channel topic — only ops can set if mode +t
typedef enum	command
{
	CAP     ,
	PASS	,
	NICK	,
	USER	,
	KICK	,
	TOPIC	,
	MODE	,
	JOIN	,
	PART	,
	PRIVMSG	,
	QUIT	,
	UNKNOWN
}				command;

struct iRCMessage
{
	std::string 				prefix;
	command						cmd;
	std::vector<std::string>	args;
	std::string 				CRLF;
	size_t						len;
	std::string					ogMess;
};


iRCMessage					parseMessage(const std::string &strMess);
bool						isMessValid(const iRCMessage &mess);
std::vector<std::string>	split(const std::string& str, char delimiter);
std::vector<std::string>	splitCRLF(const std::string& str);


#endif
