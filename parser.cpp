/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 16:34:12 by mratsima         ###   ########.fr       */
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

void	getPrefix(const std::vector<std::string> &splitMess, iRCMessage &parsedMess, size_t &index)
{
	if (splitMess.empty() || index >= splitMess.size())
		return ;
	if (!splitMess[index].empty() && splitMess[index][0] == ':')
	{
		parsedMess.prefix = splitMess[index];
		index++;
	}
}

void	getCommand(const std::vector<std::string> &splitMess, iRCMessage &parsedMess, size_t &index)
{
	std::vector<std::string>	types;

	if (splitMess.size() < index + 1)
		return ;
	std::string strCommand = splitMess[index];
	index ++;
	std::string tab[COM_NUM] = {"CAP", "PASS", "NICK", "USER", "PING", "KICK"
		, "TOPIC", "MODE", "JOIN", "PART" , "PRIVMSG", "QUIT"};
	for (int i = 0; i < COM_NUM; i++)
		types.push_back(tab[i]);
	for (unsigned int i = 0; i < types.size(); i++)
	{
		if (strCommand == types[i])
		{
			parsedMess.cmd = static_cast<command>(i);
			return ;
		}
	}
	parsedMess.cmd = UNKNOWN;
}

void	getArgs(const std::vector<std::string> &splitMess, iRCMessage &parsedMess, size_t &index)
{
	if (splitMess.size() <= index)
		return ;
	bool trailing = false;
	for (size_t i = index; i < splitMess.size(); ++i)
	{
		if (!trailing)
		{
			parsedMess.args.push_back(splitMess[i]);
			if (!splitMess[i].empty() && splitMess[i][0] == ':')
				trailing = true;
		}
		else
			parsedMess.args.back().append(" " + splitMess[i]);
		index++;
	}
}

void	getCRLF(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	if (splitMess.empty())
		return ;
	parsedMess.crlf = splitMess.back();
}

iRCMessage parseMessage(const std::string &strMess)
{
	iRCMessage					parsedMess = {"", UNKNOWN, std::vector<std::string>(), "", 0, ""};
	std::vector<std::string>	splitMess;
	size_t 						index = 0;

	if (strMess.empty())
		return (parsedMess);
	parsedMess.ogMess = strMess;
	splitMess = split(strMess, ' ');
	parsedMess.len = strMess.size();
	getPrefix(splitMess, parsedMess, index);
	getCommand(splitMess, parsedMess, index);
	getArgs(splitMess, parsedMess, index);
	getCRLF(splitMess, parsedMess);
	return (parsedMess);
}

bool	isMessValid(const iRCMessage &mess)
{
	if (mess.crlf != "/r/n")
		return (false);
	if (mess.len == 0)
		return (false);
	if (mess.args.empty() && mess.cmd == UNKNOWN && mess.crlf.empty())
		return (false);
	if (mess.cmd == UNKNOWN)
		return (false);
	if (std::count(mess.args.back().begin() + 1, mess.args.back().end(), ':') > 1)
		return (false);
	if (std::count(mess.ogMess.begin(), mess.ogMess.end(), '\r') > 1
		|| std::count(mess.ogMess.begin(), mess.ogMess.end(), '\n') > 1)
		return (false);
	if (mess.len > 512)
		return (false);
	return (true);
}

/*                 					   DEBUG                                */
// void	printiRCMESS(iRCMessage mess)
// {
// 	std::cout << "prefix = " << mess.prefix << std::endl;
// 	std::cout << "command = " << mess.command << std::endl;
// 	std::cout << "args = ";
// 	for (int i = 0; i < mess.args.size(); i++)
// 		std::cout << "-" << mess.args[i] << std::endl;
// }

// void print_vector(const std::vector<std::string>& v)
// {
//     for (size_t i = 0; i < v.size(); i++)
//         std::cout << "[" << i << "] " << v[i] << std::endl;
// }

// int main(int argc, char **argv)
// {
// 	iRCMessage stuff;

// 	if (argc < 2)
// 		return 0;
// 	stuff = parseMessage(argv[1]);
// 	print_vector(split(argv[1], ' '));
// 	printiRCMESS(stuff);
// 	return (0);
// }
// -------------------------------------------------------------------------
