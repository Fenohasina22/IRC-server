/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 16:48:12 by mratsima         ###   ########.fr       */
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

void	getPrefix(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	if (splitMess.size() < 1)
		return ;
	parsedMess.prefix = splitMess[0];
}

void	getCommand(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	if (splitMess.size() < 2)
		return ;
	std::string strCommand = splitMess[1];
	std::vector<std::string>	types = {"PASS", "NICK", "USER","KICK"
		, "TOPIC", "MODE", "JOIN", "PART" , "PRIVMSG", "QUIT"};
	for (int i = 0; i < types.size(); i++)
	{
		if (strCommand == types[i])
		{
			parsedMess.command = static_cast<command>(i);
			return ;
		}
	}
	parsedMess.command = UNKNOWN;
}

void	getArgs(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	bool trailing = false;

	if (splitMess.size() < 3)
		return ;
	for (size_t i = 2; (i < splitMess.size() && splitMess[i] != "/r/n"); i++)
	{
		if (trailing == false)
		{
			parsedMess.args.push_back(splitMess[i]);
			if (splitMess[i][0] == ':')
				trailing = true;
		}
		else
			parsedMess.args.back().append(" " + splitMess[i]);
	}
}

void	getCRLF(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	if (splitMess.empty())
		return ;
	parsedMess.CRLF = splitMess.back();
}

iRCMessage parseMessage(const std::string &strMess)
{
	iRCMessage					parsedMess = {"", UNKNOWN, {}, "", 0, ""};
	std::vector<std::string>	splitMess;
	int							index = 0;

	if (strMess.empty())
		return (parsedMess);
	parsedMess.ogMess = strMess;
	splitMess = split(strMess, ' ');
	parsedMess.len = strMess.size();
	getPrefix(splitMess, parsedMess);
	getCommand(splitMess, parsedMess);
	getArgs(splitMess, parsedMess);
	getCRLF(splitMess, parsedMess);
	return (parsedMess);
}

bool	isMessValid(const iRCMessage &mess)
{
	if (mess.CRLF != "/r/n")
		return (false);
	if (mess.len == 0)
		return (false);
	if (mess.args.empty() && mess.command == UNKNOWN && mess.CRLF.empty()
		&& mess.prefix.empty())
		return (false);
	if (mess.command == UNKNOWN)
		return (false);
	if (std::count(mess.args.back().begin(), mess.args.back().end(), ':') > 1)
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
