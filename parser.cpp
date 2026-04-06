/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/06 12:20:38 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string>	tokens;
    std::string 				token;
    std::stringstream			ss(str);

    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return (tokens);
}

void	getPrefix(const std::vector<std::string> &splitMess, message &parsedMess)
{
	parsedMess.prefix = splitMess[0];
}

void	getCommand(const std::vector<std::string> &splitMess, message &parsedMess)
{
	std::string strCommand = splitMess[1];
	std::vector<std::string>	types = {"/kick", "/topic", "/mode", "/join"
		, "/part", "/pivmsg", "/topic", "/quit"};
	for (int i = 0; i < types.size(); i++)
	{
		if (strCommand == types[i])
			parsedMess.command = static_cast<command>(i);
	}
	parsedMess.command = UNKNOWN;
}

void	getArgs(const std::vector<std::string> &splitMess, message &parsedMess)
{
	for (int i = 1; (i < splitMess.size() && splitMess[i] != "/r/n"); i++)
		parsedMess.args.push_back(splitMess[i]);
}

void	getCRLF(const std::vector<std::string> &splitMess, message &parsedMess)
{
	parsedMess.CRLF = splitMess.back();
}

message parseMessage(const std::string &strMess)
{
	message						parsedMess;
	std::vector<std::string>	splitMess;
	int							index = 0;

	if (strMess.empty())
		return {"", UNKNOWN, {}, "", 0};
	splitMess = split(strMess, ' ');
	parsedMess.len = strMess.size();
	getPrefix(splitMess, parsedMess);
	getCommand(splitMess, parsedMess);
	getArgs(splitMess, parsedMess);
	getCRLF(splitMess, parsedMess);
}
