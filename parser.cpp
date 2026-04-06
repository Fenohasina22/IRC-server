/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/06 09:41:34 by mratsima         ###   ########.fr       */
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

void	getPrefix(const std::vector<std::string> &splitMess, message &parsed)
{
	parsed.prefix = splitMess[0];
}

void	getCommand(const std::vector<std::string> &splitMess, message &parsed)
{
	parsed.command = splitMess[1];
}

void	getArgs(const std::vector<std::string> &splitMess, message &parsed)
{
	parsed.command = splitMess[1];
}

message parseMessage(const std::string &strMess)
{
	message						parsedMess;
	std::vector<std::string>	splitMess;
	int							index = 0;

	if (strMess.empty())
		return {"", "", "", 0};
	splitMess = split(strMess, ' ');
	getPrefix(splitMess, parsedMess);
	getCommand(splitMess, parsedMess);
	getArguments(splitMess, parsedMess);
	getCRLF;
}
