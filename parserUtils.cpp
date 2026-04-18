/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 19:01:29 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 16:08:58 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

std::string		strtrim(std::string &str)
{
	size_t	begin;
	size_t	last;
	std::string	cleaned;

	begin = str.find_first_not_of(" \t");
	if (begin == std::string::npos)
	{
		begin = 0;
	}
	last = str.find_last_not_of(" \t");
	if (last == std::string::npos)
	{
		last = str.size() - 1;
	}
	cleaned = str.substr(begin, last + 1);
	return (cleaned);
}

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string>	tokens;
    std::string 				token;
    std::stringstream			ss(str);

    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return (tokens);
}

std::vector<std::string> splitCRLF(const std::string& str)
{
	std::vector<std::string> parts;
	size_t pos = 0;
	size_t start = 0;
	const std::string delim = "\r\n";
	while ((pos = str.find(delim, start)) != std::string::npos)
	{
		parts.push_back(str.substr(start, pos - start));
		start = pos + delim.length();
	}
	if (start < str.size())
		parts.push_back(str.substr(start));
	return parts;
}

void	createTypeTab(std::vector<std::string>	&types)
{
	std::string tab[COM_NUM] = {"CAP", "PASS", "NICK", "USER", "PING", "KICK"
		, "TOPIC", "MODE", "JOIN", "PART" , "PRIVMSG", "INVITE", "QUIT"
		, "DISCONNECT"};
	for (int i = 0; i < COM_NUM; i++)
		types.push_back(tab[i]);
}

void	assignCommand(std::vector<std::string>	&t, std::string	&cmd, iRCMessage &msg)
{
	for (unsigned int i = 0; i < t.size(); i++)
	{
		if (cmd == t[i])
		{
			msg.cmd = static_cast<command>(i);
			return ;
		}
	}
	msg.cmd = UNKNOWN;
}

void	handleTrailingArgs(
	iRCMessage 						&parsedMess,
	std::vector<std::string>		&splitMess,
	bool 							&trailing,
	size_t 							&i)
{
	std::string copy = splitMess[i];
	if (copy.size() >= 2 && copy.find(CRLF) != copy.npos
	&& copy.find(CRLF)== copy.size() - 2)
	{
		copy.resize(copy.size() - 2);
		splitMess[i] = copy;
	}
	parsedMess.args.push_back(splitMess[i]);
	if (!splitMess[i].empty() && splitMess[i][0] == ':')
		trailing = true;
}

void	initialiseIRCMessage(iRCMessage &msg)
{
    msg.prefix = "";
    msg.cmd = UNKNOWN;
	msg.strCmd = "";
    msg.args = std::vector<std::string>();
    msg.crlf = "";
    msg.len = 0;
    msg.ogMess = "";
}
