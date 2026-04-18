/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 19:20:00 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

void	getPrefix(
	const std::vector<std::string>	&splitMess,
	iRCMessage						&parsedMess,
	size_t							&index)
{
	if (splitMess.empty() || index >= splitMess.size())
		return ;
	if (!splitMess[index].empty() && splitMess[index][0] == ':')
	{
		parsedMess.prefix = splitMess[index];
		index++;
	}
}

void	getCommand(
	std::vector<std::string>	&splitMess,
	iRCMessage 						&parsedMess,
	size_t 							&index)
{
	std::vector<std::string>	types;
	std::string					strCommand;

	if (splitMess.size() < index + 1)
		return ;
	std::string copy = splitMess[index];
	if (copy.size() >= 2 && copy.find(CRLF) != copy.npos
	&& copy.find(CRLF)== copy.size() - 2)
	{
		copy.resize(copy.size() - 2);
		splitMess[index] = copy;
	}
	strCommand = splitMess[index];
	parsedMess.strCmd = strCommand;
	index ++;
	createTypeTab(types);
	assignCommand(types, strCommand, parsedMess);
}

void	getArgs(
	std::vector<std::string>		&splitMess,
	iRCMessage 						&parsedMess,
	size_t							&index)
{
	bool trailing = false;

	if (splitMess.size() <= index)
		return ;
	for (size_t i = index; i < splitMess.size(); ++i)
	{
		if (!trailing)
			handleTrailingArgs(parsedMess, splitMess, trailing, i);
		else
			parsedMess.args.back().append(" " + splitMess[i]);
		index++;
	}
}

void	getCRLF(iRCMessage &parsedMess)
{
	if (parsedMess.ogMess.size() >= 2
		&& parsedMess.ogMess.substr(parsedMess.ogMess.size() - 2) == CRLF)
		parsedMess.crlf = CRLF;
	else
		parsedMess.crlf.clear();
}

iRCMessage parseMessage(const std::string &strMess)
{
	iRCMessage					parsedMess;
	std::vector<std::string>	splitMess;
	size_t 						index = 0;

	initialiseIRCMessage(parsedMess);
	if (strMess.empty())
		return (parsedMess);
	parsedMess.ogMess = strMess;
	splitMess = split(strMess, ' ');
	parsedMess.len = strMess.size();
	getPrefix(splitMess, parsedMess, index);
	getCommand(splitMess, parsedMess, index);
	getArgs(splitMess, parsedMess, index);
	getCRLF(parsedMess);
	return (parsedMess);
}

bool	isMessValid(iRCMessage &mess)
{
	if (mess.crlf != "\r\n")
		return (false);
	if (mess.len == 0)
		return (false);
	if (mess.len > 512)
		return (false);
	return (true);
}
