/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:40 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/20 11:30:06 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <sstream>
# include <vector>
# include <string>
# include <algorithm>
# include <climits>

# ifndef COM_NUM
#  define COM_NUM 14
# endif

# ifndef	CRLF
# define CRLF "\r\n"
# endif

# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[1;36m"
# define RESET   "\033[0m"

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
	std::string					strCmd;
	std::vector<std::string>	args;
	std::string 				crlf;
	size_t						len;
	std::string					ogMess;
	bool						has_trailing;
};


void						initialiseIRCMessage(iRCMessage &msg);
iRCMessage					parseMessage(const std::string &strMess);
bool						isMessValid(iRCMessage &mess);
std::vector<std::string>	split(const std::string& str, char delimiter);
std::vector<std::string>	splitCRLF(const std::string& str);
void						createTypeTab(std::vector<std::string>	&types);
void						assignCommand(std::vector<std::string>	&t, std::string	&cmd, iRCMessage &msg);
void						handleTrailingArgs(iRCMessage &parsedMess, std::vector<std::string>	&splitMess,
							bool &trailing,size_t &i);
std::string					strtrim(std::string &str);
#endif
