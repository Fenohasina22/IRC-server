/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/06 13:19:04 by mratsima         ###   ########.fr       */
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
	std::vector<std::string>	types = {"KICK", "TOPIC", "MODE", "JOIN", "PART"
		, "PRIVMSG", "QUIT"};
	for (int i = 0; i < types.size(); i++)
	{
		if (strCommand == types[i])
			parsedMess.command = static_cast<command>(i);
	}
	parsedMess.command = UNKNOWN;
}

void	getArgs(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	if (splitMess.size() < 3)
		return ;
	for (int i = 2; (i < splitMess.size() && splitMess[i] != "/r/n"); i++)
		parsedMess.args.push_back(splitMess[i]);
}

void	getCRLF(const std::vector<std::string> &splitMess, iRCMessage &parsedMess)
{
	if (splitMess.empty())
		return ;
	parsedMess.CRLF = splitMess.back();
}

iRCMessage parseMessage(const std::string &strMess)
{
	iRCMessage					parsedMess = {"", UNKNOWN, {}, "", 0};
	std::vector<std::string>	splitMess;
	int							index = 0;

	if (strMess.empty())
		return (parsedMess);
	splitMess = split(strMess, ' ');
	parsedMess.len = strMess.size();
	getPrefix(splitMess, parsedMess);
	getCommand(splitMess, parsedMess);
	getArgs(splitMess, parsedMess);
	getCRLF(splitMess, parsedMess);
	return (parsedMess);
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
