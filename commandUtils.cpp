/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:10:46 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/15 07:50:05 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void 	tryRegistration(Client &client, Server& serv)
{
	if (client.isRegistered())
		return ;
	if (client.getNickState() && client.getPassState() && client.getUserState())
	{
		client.setRegistrationState(true);
		std::string msg001 = ":server 001 " + client.getNick() + " :Welcome to the IRC " + client.getNick() + CRLF;
		std::string msg002 = ":server 002 " + client.getNick() + " :Your host is server" + CRLF;
		std::string msg003 = ":server 003 " + client.getNick() + " :This server was created just now" + CRLF;
		std::string msg004 = ":server 004 " + client.getNick() + " idk bro put all the infos abt our server" + CRLF;
		std::string all = msg001 + msg002 + msg003 + msg004;

		client.ConcatenateWBuffer(all, serv);
	}
}

std::string	formMess(const Client	&sender,const Client &destCli,const iRCMessage &mess)
{
	std::string 	messageOutput;

	messageOutput += ":" + sender.getNick();
	messageOutput += " PRIVMSG ";
	messageOutput += destCli.getNick() + " ";
	messageOutput += mess.args[1];
	messageOutput += CRLF;
	return (messageOutput);
}

std::string	formChanMess(const Client	&sender,const Channel &destChan,const iRCMessage &mess)
{
	std::string 	messageOutput;

	messageOutput += ":" + sender.getNick();
	messageOutput += " PRIVMSG ";
	messageOutput += destChan.getName() + " ";
	messageOutput += mess.args[1];
	messageOutput += CRLF;
	return (messageOutput);
}

bool 	chanExists(const std::string &chanName, Server &serv)
{
	bool	foundChan;

	serv.findChan(chanName, foundChan);
	if (foundChan)
		return (true);
	return (false);
}

std::string	formNameList(Channel &destChan)
{
	std::set<std::string> 			members = destChan.getMembers();
	std::set<std::string>::iterator itBegin = members.begin();
	std::set<std::string>::iterator itEnd 	= members.end();
	std::string						nameList;
	for (std::set<std::string>::iterator it = itBegin; it != itEnd ; it++)
	{
		std::string name = *it;
		if (destChan.isOps(name))
			name = "@" + name;
		nameList.append(name + " ");
	}
	if (!nameList.empty())
		nameList.resize(nameList.size() - 1);
	return (nameList);
}

void	sendChannelState(Client &client, Channel &destChan, Server &serv)
{
	std::string mess;

	if (destChan.getTopic() == "")
	{
		// 331 nick #channel :No topic is set
		client.ConcatenateWBuffer(FormatedMessage("331", ":server"
			, client.getNick() + " " + destChan.getName() + " :No topic is set"), serv);
	}
	else
	{
		client.ConcatenateWBuffer(FormatedMessage("332", ":server"
			, client.getNick() + " " + destChan.getName()
			+ " :" + destChan.getTopic()), serv);
	}
	std::string nameList = formNameList(destChan);
	mess += client.getNick();
	mess += " = ";
	mess += destChan.getName();
	mess += " :" + nameList;
	client.ConcatenateWBuffer(FormatedMessage("353", ":server", mess), serv);
	mess.clear();
	mess += client.getNick() + " ";
	mess += destChan.getName();
	mess += " :End of /NAMES list";
	client.ConcatenateWBuffer(FormatedMessage("366", ":server", mess), serv);
}

ChanModes strToMode(std::string strMode, ModeAction &action)
{
	if (strMode.size() < 2)
		action = NO_ACTION;
	else if (strMode[0] == '+')
		action = ADD;
	else if (strMode[0] == '-')
		action = REMOVE;
	else
		action = NO_ACTION;
	if (strMode == "+i"  || strMode == "-i")
		return (i);
	else if (strMode == "+o"  || strMode == "-o")
		return (o);
	else if (strMode == "+t"  || strMode == "-t")
		return (t);
	else if (strMode == "+k"  || strMode == "-k")
		return (k);
	else if (strMode == "+l"  || strMode == "-l")
		return (l);
	else
		return (unknown);
}

void	doIflag(Channel &destChan, ModeAction &act)
{
	if (act == ADD)
	{
		destChan.addFlag("i");
		destChan.setInviteOnly(true);
	}
	else
	{
		destChan.removeFlag("i");
		destChan.setInviteOnly(false);
	}
}

void	doTflag(Channel &destChan, ModeAction &act)
{
	if (act == ADD)
	{
		destChan.addFlag("t");
		destChan.setTopicLocked(true);
	}
	else
	{
		destChan.removeFlag("t");
		destChan.setTopicLocked(false);
	}
}

bool	doKflag(Channel &destChan, ModeAction &act, std::vector<std::string> &args)
{
	if (act == ADD)
	{
		if (args.size() < 3)
			return (false);
		destChan.addFlag("k");
		destChan.setPass(args[2]);
		destChan.setPassRequired(true);
	}
	else
	{
		destChan.removeFlag("k");
		destChan.setPass("");
		destChan.setPassRequired(false);
	}
	return (true);
}

bool	doLflag(Channel &destChan, ModeAction &act, std::vector<std::string> &args)
{
	int 	newLimit = 0;
	char	*endptr;
	if (act == ADD)
	{
		if (args.size() < 3)
			return (false);
		errno = 0;
		newLimit = std::strtol(args[2].c_str(), &endptr, 10);
		if (*endptr || endptr == args[2])
			return (false);
		if (errno)
			return (false);
		if (newLimit < 0 || newLimit > 9999)
			return (false);
		if (newLimit > 512)
			newLimit = 512;
		destChan.addFlag("l");
		destChan.setMaxUser(newLimit);
		destChan.setUserLimitEnabled(true);
	}
	else
	{
		destChan.removeFlag("l");
		destChan.setMaxUser(512);
		destChan.setUserLimitEnabled(false);
	}
	return (true);
}

int	doOflag(Channel &destChan, ModeAction &act, std::vector<std::string> &args, Client &client, Server &serv)
{
	bool	foundTarget = false;
	Client	target 		= serv.findClient(args[1], foundTarget);

	if (!client.isInChannel(destChan.getName()))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":server", client.getNick() + " " + destChan.getName() + " :You're not on that channel"), serv);
		return (1);
	}
	if (!destChan.isOps(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":server", client.getNick() + " " + destChan.getName() + " :You're not a channel operator"), serv);
		return (2);
	}
	if (args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", client.getNick() + " MODE :Not enough parameters"), serv);
		return (3);
	}
	if (!foundTarget)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + args[1] + " :No such nick"), serv);
		return (4);
	}
	if (!target.isInChannel(destChan.getName()))
	{
		client.ConcatenateWBuffer(FormatedMessage("441", ":server", target.getNick() + " " + destChan.getName() + " :They aren't on that channel"), serv);
		return (5);
	}
	if (act == ADD)
		destChan.addOperator(&target);
	else
		destChan.removeOperator(&target);
	return (0);
}

bool	isNicknameInUse(Server &serv, Client &client, std::string &newNick)
{
	for (unsigned int i = 0; i < serv.getAllClients().size(); i++)
	{
		if (serv.getAllClients()[i].getNick() == newNick)
		{
			if (!client.isRegistered())
			{
				client.ConcatenateWBuffer(FormatedMessage("433", ":server",
					 "* " + newNick + " :Nickname is already in use"), serv);
			}
			else
			{
				client.ConcatenateWBuffer(FormatedMessage("433", ":server",
					 client.getNick() + " " + newNick + " :Nickname is already in use"), serv);
			}
			return (true);
		}
	}
	return (false);
}
