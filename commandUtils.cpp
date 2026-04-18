/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:10:46 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 18:16:23 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "commands.hpp"
#include <climits>

bool	IsValidNick(std::string nick, Client&  client, Server& serv)
{
	std::string	a = "@!*?$#, .";
	size_t pos;
	unsigned int i;

	i = 0;
	while (i < a.size())
	{
		pos = nick.find(a[i]);
		if (pos != std::string::npos)
		{
			std::cout << RED << "INVALID NICKNAME WAS ENTERED" << RESET << std::endl;
			client.ConcatenateWBuffer(FormatedMessage("432", ":" + serv.getName(),
				 "* " + nick + " :Erroneous nickname" ), serv);
			return (false);
		}
		i++;
	}
	return (true);
}

void	PrintClients(std::vector<Client>& container)
{
	std::cout << " === fds ===" << std::endl;
	for (unsigned int i = 0; i < container.size(); i++)
	{
		std::cout << "["<<container[i].getFd() << "] ";
	}
	std::cout << std::endl;
}

void	CleanUp(Server& serv, int i)
{
	bool	foundC;
	int		saveFd;

	saveFd = serv.getVecPoll()[i].fd;
	Client	&c = serv.findTrueClient(saveFd, foundC);
	if (!foundC)
	{
		DeleteVecElement(serv.getVecPoll(), saveFd);
		DeleteVecElementClient(serv.getAllClients(), saveFd);
		close (saveFd);
		return ;
	}
	std::set<std::string> Chans = c.getJoinedChannels();
	for (std::set<std::string>::iterator it = Chans.begin();
	 it != Chans.end(); it++)
	{
		bool	foundChan;
		Channel &tmpChan = serv.findChan(*it, foundChan);
		tmpChan.removeClient(&c);
	}
	DeleteVecElement(serv.getVecPoll(), saveFd);
	DeleteVecElementClient(serv.getAllClients(), saveFd);
	DeleteVecElementClient(serv.getTrueClients(), saveFd);
	close (saveFd);
	return ;
}

void 	tryRegistration(Client &client, Server& serv)
{
	if (client.isRegistered())
		return ;
	if (client.getNickState() && client.getPassState() && client.getUserState())
	{
		client.setRegistrationState(true);
		std::string msg001 = ":" + serv.getName() + " 001 " + client.getNick() + " :Welcome to the " + serv.getName() + " " + client.getNick() + CRLF;
		std::string msg002 = ":" + serv.getName() + " 002 " + client.getNick() + " :Your host is " + serv.getName() + CRLF;
		std::string msg003 = ":" + serv.getName() + " 003 " + client.getNick() + " :This server was created just now" + CRLF;
		std::string msg004 = ":" + serv.getName() + " 004 " + client.getNick() + " " +  serv.getName() + " 1.0 o itkol"+ CRLF;
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
		client.ConcatenateWBuffer(FormatedMessage("331", ":" + serv.getName()
			, client.getNick() + " " + destChan.getName() + " :No topic is set"), serv);
	}
	else
	{
		client.ConcatenateWBuffer(FormatedMessage("332", ":" + serv.getName()
			, client.getNick() + " " + destChan.getName()
			+ " :" + destChan.getTopic()), serv);
	}
	std::string nameList = formNameList(destChan);
	mess += client.getNick();
	mess += " = ";
	mess += destChan.getName();
	mess += " :" + nameList;
	client.ConcatenateWBuffer(FormatedMessage("353", ":" + serv.getName(), mess), serv);
	mess.clear();
	mess += client.getNick() + " ";
	mess += destChan.getName();
	mess += " :End of /NAMES list";
	client.ConcatenateWBuffer(FormatedMessage("366", ":" + serv.getName(), mess), serv);
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
		if (args.size() < 1)
			return (false);
		destChan.addFlag("k");
		destChan.setPass(args[0]);
		args.erase(args.begin());
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
	int     newLimit = 0;
	char    *endptr = NULL;
	long    testLimit = 0;

	if (act == ADD)
	{
		if (args.size() < 1)
			return (false);
		const char *s = args[0].c_str();
		testLimit = std::strtol(s, &endptr, 10);
		if (endptr == s || *endptr != '\0')
			return (false);
		if (testLimit == LONG_MAX || testLimit == LONG_MIN)
			return (false);
		if (testLimit < 0 || testLimit > 9999)
			return (false);
		newLimit = static_cast<int>(testLimit);
		if (newLimit > 512)
			newLimit = 512;
		args.erase(args.begin());
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
	if (args.size() < 1)
	{
		client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
			 client.getNick() + " MODE :Not enough parameters"), serv);
		return (3);
	}

	bool	foundTarget = false;
	Client	target 		= serv.findTrueClient(args[0], foundTarget);

	if (!client.isInChannel(destChan.getName()))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":" + serv.getName(),
			 client.getNick() + " " + destChan.getName()
			 + " :You're not on that channel"), serv);
		return (1);
	}
	if (!destChan.isOps(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":" + serv.getName(),
			 client.getNick() + " " + destChan.getName()
			 + " :You're not a channel operator"), serv);
		return (2);
	}
	if (!foundTarget)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":" + serv.getName(),
			 client.getNick() + " " + args[0] + " :No such nick"), serv);
		return (4);
	}
	if (!target.isInChannel(destChan.getName()))
	{
		client.ConcatenateWBuffer(FormatedMessage("441", ":" + serv.getName(),
			 client.getNick() + " " + target.getNick() + " " + destChan.getName()
			 + " :They aren't on that channel"), serv);
		return (5);
	}
	if (act == ADD)
		destChan.addOperator(&target);
	else
		destChan.removeOperator(&target);
	args.erase(args.begin());
	return (0);
}

bool	isNicknameInUse(Server &serv, Client &client, std::string &newNick)
{
	for (unsigned int i = 0; i < serv.getTrueClients().size(); i++)
	{
		if (serv.getTrueClients()[i].getNick() == newNick)
		{
			if (!client.isRegistered())
			{
				client.ConcatenateWBuffer(FormatedMessage("433", ":" + serv.getName(),
					 "* " + newNick + " :Nickname is already in use"), serv);
			}
			else
			{
				client.ConcatenateWBuffer(FormatedMessage("433", ":" + serv.getName(),
					 client.getNick() + " " + newNick + " :Nickname is already in use"), serv);
			}
			return (true);
		}
	}
	return (false);
}

void	DeleteVecElement(std::vector<pollfd>& vec, int fd)
{
	std::vector<pollfd>::iterator it;
	for (it = vec.begin(); it != vec.end();)
	{
		if ((*it).fd == fd)
		{
			it = vec.erase(it);
		}
		else
			++it;
	}
}

void	DeleteVecElementClient(std::vector<Client>& vec, int fd)
{
	std::vector<Client>::iterator it;
	for (it = vec.begin(); it != vec.end();)
	{
		if ((*it).getFd() == fd)
		{
			it = vec.erase(it);
		}
		else
			++it;
	}
}


bool	getNeighbors(Client &client, Server &serv, std::set<std::string> &membersToNotify)
{
	std::set<std::string> joined = client.getJoinedChannels();
	for (std::set<std::string>::iterator it = joined.begin(); it != joined.end(); it++)
	{
		bool success;
		Channel& chan = serv.findChan(*it, success);
		if (success)
		{
			std::set<std::string>::iterator it;
			std::set<std::string>::iterator itBegin = chan.getMembers().begin();
			std::set<std::string>::iterator itEnd = chan.getMembers().end();;
			for (it = itBegin; it != itEnd; it++)
			{
				membersToNotify.insert(*it);
			}
		}
		else
		{
			return (false);
		}
	}
	membersToNotify.erase(client.getNick());
	return (true);
}

void updateInvitedAcrossAll(Server &serv, const std::string &oldNick, const std::string &newNick)
{
    std::vector<Channel> &chans = serv.getAllChans();
    for (size_t i = 0; i < chans.size(); ++i)
    {
        std::set<std::string> &inv = chans[i].getInvited();
        if (inv.erase(oldNick))
            inv.insert(newNick);
    }
}

bool	updateChannels(Client &client, Server &serv, std::string &newNick)
{
	std::set<std::string> joined = client.getJoinedChannels();
	for (std::set<std::string>::iterator it = joined.begin(); it != joined.end(); it++)
	{
		bool success;
		Channel& chan = serv.findChan(*it, success);
		std::set<std::string> &members = chan.getMembers();
		std::set<std::string> &ops = chan.getOps();
		if (success)
		{
			std::set<std::string>::iterator itMember;
			std::set<std::string>::iterator itOps;
			itMember = members.find(client.getNick());
			itOps = ops.find(client.getNick());
			if (itMember != members.end())
			{
				members.erase(itMember);
				members.insert(newNick);
			}
			if (itOps != ops.end())
			{
				ops.erase(itOps);
				ops.insert(newNick);
			}
		}
		else
		{
			return (false);
		}
	}
	updateInvitedAcrossAll(serv, client.getNick(), newNick);
	return (true);
}



bool	notifyNeighbors(Client &client, Server &serv, std::string &newNick)
{
	std::string				msg;
	std::set<std::string>	membersToNotify;
	struct sockaddr_in		tmp;
	char					hostname[INET_ADDRSTRLEN];

	tmp = client.getClientInfos();
	inet_ntop(AF_INET, &(tmp.sin_addr), hostname, INET_ADDRSTRLEN);
	msg += ":" + client.getNick() + "!" + client.getUser() + "@" + hostname;
	msg += " NICK :";
	msg += newNick;
	msg += CRLF;

	if (!getNeighbors(client, serv, membersToNotify))
		return (false);
	client.ConcatenateWBuffer(msg, serv);
	serv.broadcastWithoutChan(msg, client, membersToNotify, serv);
	return (true);
}

void	privmsgToChan(
	Client		&sender,
	Channel 	&destChan,
	Server		&serv,
	iRCMessage	&mess,
	std::string &messageOutput)
{
	std::set<std::string> members = destChan.getMembers();
		for (std::set<std::string>::iterator it = members.begin(); it != members.end(); ++it)
		{
			bool found = false;
			Client &target = serv.findTrueClient(*it, found);
			if (!found)
				continue;
			messageOutput = formChanMess(sender, destChan, mess);
			if (target.getNick() != sender.getNick())
				target.ConcatenateWBuffer(messageOutput, serv);
		}
}

bool	checkChanRestrictions(Client &client, iRCMessage &mess, Server serv, Channel *destChan)
{
	if (client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("443", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :is already on channel"), serv);
		return (false);
	}
	if (destChan->isInviteOnly() && !destChan->isInvited(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("473", ":" + serv.getName(),
			 client.getNick() + " " + destChan->getName()
			 + " :cannot join channel (+i)"), serv);
		return (false);
	}
	if (destChan->isPassRequired() && (mess.args.size() < 2 || mess.args[1] != destChan->getPass()))
	{
		client.ConcatenateWBuffer(FormatedMessage("475", ":" + serv.getName(),
			 client.getNick() + " " + destChan->getName() +
			  " :cannot join channel (+k)"), serv);
		return (false);
	}
	if (static_cast<int>(destChan->getMembers().size() + 1) > destChan->getMaxUser())
	{
		client.ConcatenateWBuffer(FormatedMessage("471", ":" + serv.getName(),
			 client.getNick() + " " + destChan->getName() + " :cannot join channel (+l)"), serv);
		return (false);
	}
	return (true);
}

bool	checkChanRestrictions(Client &client, Server &serv, iRCMessage &mess, Channel *destChan)
{
	if (client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("443", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :is already on channel"), serv);
		return (false);
	}
	if (destChan->isInviteOnly() && !destChan->isInvited(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("473", ":" + serv.getName(),
			 client.getNick() + " " + destChan->getName()
			 + " :cannot join channel (+i)"), serv);
		return (false);
	}
	if (destChan->isPassRequired() && (mess.args.size() < 2 || mess.args[1] != destChan->getPass()))
	{
		client.ConcatenateWBuffer(FormatedMessage("475", ":" + serv.getName(),
			 client.getNick() + " " + destChan->getName() +
			  " :cannot join channel (+k)"), serv);
		return (false);
	}
	if (static_cast<int>(destChan->getMembers().size() + 1) > destChan->getMaxUser())
	{
		client.ConcatenateWBuffer(FormatedMessage("471", ":" + serv.getName(),
			 client.getNick() + " " + destChan->getName() + " :cannot join channel (+l)"), serv);
		return (false);
	}
	return (true);
}

void	broadcastJoin(std::string &broadcastMess, Client &client, Server &serv, Channel *destChan)
{
	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@" + CurrentHostname(client);
	broadcastMess += " JOIN ";
	broadcastMess += destChan->getName();
	broadcastMess += CRLF;
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, *destChan, serv);
}


bool	processTopicCommand(
	bool &displayTopic,
	bool &changeTopic,
	bool &resetTopic,
	Client &client,
	iRCMessage &mess,
	Server &serv,
	Channel &destChan)
{
	if (mess.args.size() == 1)
		displayTopic = true;
	else if (mess.args.size() > 1 && mess.args[1] == ":")
		resetTopic = true;
	else if (mess.args.size() > 1)
		changeTopic = true;
	if (displayTopic)
	{
		if (destChan.getTopic() == "")
		{
			client.ConcatenateWBuffer(FormatedMessage("331", ":" + serv.getName(),
				 client.getNick() + " " + destChan.getName() + " :No topic is set"), serv);
			return (false);
		}
		client.ConcatenateWBuffer(FormatedMessage("332", ":" + serv.getName(),
			 client.getNick() + " " + destChan.getName() + " :" + destChan.getTopic()), serv);
	}
	else if (changeTopic)
	{
		if (destChan.isTopicLocked() && !destChan.isOps(client.getNick()))
		{
			client.ConcatenateWBuffer(FormatedMessage("482", ":" + serv.getName(),
				 client.getNick() + " " + destChan.getName() +
				  " :You're not a channel operator"), serv);
			return (false);
		}
		if (!client.isInChannel(destChan.getName()))
		{
			client.ConcatenateWBuffer(FormatedMessage("442",
				 ":" + serv.getName(), client.getNick() + " " + mess.args[0] +
				 " :You're not on that channel"), serv);
			return (false);
		}
	}
	else if (resetTopic)
	{
		if (destChan.isTopicLocked() && !destChan.isOps(client.getNick()))
		{
			client.ConcatenateWBuffer(FormatedMessage("482",
				 ":" + serv.getName(), client.getNick() + " " + destChan.getName() +
				 " :You're not a channel operator"), serv);
			return (false);
		}
		if (!client.isInChannel(destChan.getName()))
		{
			client.ConcatenateWBuffer(FormatedMessage("442", ":" + serv.getName(),
				 client.getNick() + " " + mess.args[0] +
				  " :You're not on that channel"), serv);
			return (false);
		}
	}
	return (true);
}

void	broadcastTopicChange(
	std::string &broadcastMess,
	Client &client,
	Channel &destChan,
	Server &serv)
{
	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@" + CurrentHostname(client);
	broadcastMess += " TOPIC ";
	broadcastMess += destChan.getName();
	broadcastMess += " :" + destChan.getTopic();
	broadcastMess += CRLF;
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, destChan, serv);
}

void	updateTopic(
	bool &changeTopic,
	bool &resetTopic,
	iRCMessage &mess,
	Channel &destChan,
	std::string &broadcastMess,
	Client &client, Server &serv)
{
	if (changeTopic || resetTopic)
	{
		std::string newTopic;
		if (resetTopic)
			newTopic = "";
		else
			newTopic = mess.args[1];
		if (!newTopic.empty() && newTopic[0] == ':')
			newTopic = newTopic.substr(1, newTopic.size());
		destChan.setTopic(newTopic);
		broadcastTopicChange(broadcastMess, client, destChan, serv);
	}
}

bool	checkChannelAccess(bool &foundChan, Client &client, iRCMessage &mess, Server &serv, Client &destCli, Channel &destChan, bool &foundCli)
{
	if (!foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (!client.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :You're not on that channel"), serv);
		return (false);
	}
	if (!foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[1] + " :No such nick/channel"), serv);
		return (false);
	}
	if (!destCli.isInChannel(mess.args[0]))
	{
		client.ConcatenateWBuffer(FormatedMessage("441", ":" + serv.getName(),
			 destCli.getNick() + " " + mess.args[0] + " :They aren't on that channel"), serv);
		return (false);
	}
	if (!destChan.isOps(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :You're not a channel operator"), serv);
		return (false);
	}
	return (true);
}

void	broadCastKick(
	std::string &broadcastMess,
	Client 		&client,
	Client 		&destCli,
	Channel 	&destChan,
	iRCMessage 	&mess,
	Server 		&serv)
{
	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@" + CurrentHostname(client);
	broadcastMess += " KICK ";
	broadcastMess += destChan.getName() + " ";
	broadcastMess += destCli.getNick();
	if (mess.args.size() > 2)
	{
		std::string reason = mess.args[2];
		if (!reason.empty() && reason[0] == ':')
			reason = reason.substr(1);
		broadcastMess += " :" + reason;
	}
	broadcastMess += CRLF;
	client.ConcatenateWBuffer(broadcastMess, serv);
	serv.broadcast(broadcastMess, client, destChan, serv);
}

bool	validateChannelModeAccess(Server &serv, Client &client, iRCMessage &mess)
{
	bool	foundChan = false;
	bool	foundCli = false;
	Channel &destChan = serv.findChan(mess.args[0], foundChan);
	serv.findTrueClient(mess.args[0], foundCli);
	if (!foundChan && !foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :No such channel"), serv);
		return (false);
	}
	if (foundCli)
		return (false);
	if (mess.args.size() < 2)
	{
		client.ConcatenateWBuffer(FormatedMessage("324", ":" + serv.getName(),
			 client.getNick() + " " + destChan.getName() + " " + destChan.flagsToStr()), serv);
		return (false);
	}
	if (!destChan.isOps(client.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :You're not a channel operator"), serv);
		return (false);
	}
	return (true);
}

bool	processModeChange(
	std::vector<std::string> &args,
	std::string	&modeChange,
	ChanModes 	&mode,
	ModeAction 	&act,
	Channel		&destChan,
	Client 		&client,
	Server		&serv)
{
	mode = strToMode(modeChange, act);
	if (mode == unknown || act == NO_ACTION)
	{
		client.ConcatenateWBuffer(FormatedMessage("472", ":" + serv.getName(),
			 client.getNick() + " " + modeChange + " :is unknown mode char to me"), serv);
		return (false);
	}
	switch (mode)
	{
		case i:
			doIflag(destChan, act);
			break;

		case t:
			doTflag(destChan, act);
			break;

		case k:
			if (!doKflag(destChan, act, args))
			{
				client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
					 client.getNick() + " MODE :Not enough parameters"), serv);
				return (false);
			}
			break;

		case l:
			if (!doLflag(destChan, act, args))
			{
				client.ConcatenateWBuffer(FormatedMessage("461", ":" + serv.getName(),
					 client.getNick() + " MODE :Not enough parameters"), serv);
				return (false);
			}
			break;
		case o:
			if (doOflag(destChan, act, args, client, serv))
				return (false);
		default:
			break;
	}
	return (true);
}

void	broacastModeChange(Client &client, Channel &destChan, const std::string &finalModeStr, Server &serv)
{
	std::string broadcastMess;

	broadcastMess += ":" + client.getNick() + "!" + client.getUser() + "@host";
	broadcastMess += " MODE ";
	broadcastMess += destChan.getName() + " ";
	broadcastMess += finalModeStr;
	broadcastMess += CRLF;

	serv.broadcast(broadcastMess, client, destChan, serv);
	client.ConcatenateWBuffer(broadcastMess, serv);
}

bool	validateInvite(
	bool 		&foundCli,
	Client		&client,
	bool 		&foundChan,
	Client 		&senderCli,
	iRCMessage 	&mess,
	Channel		&destChan,
	Server 		&serv,
	Client 		&invitedCli)
{
	if (!foundCli)
	{
		client.ConcatenateWBuffer(FormatedMessage("401", ":" + serv.getName(),
			 client.getNick() + " " + mess.args[0] + " :No such nick"), serv);
		return (false);
	}
	if (!foundChan)
	{
		client.ConcatenateWBuffer(FormatedMessage("403", ":" + serv.getName(),
      	  client.getNick() + " " + mess.args[1] + " :No such channel"), serv);
		return (false);
	}
	if (!senderCli.isInChannel(mess.args[1]))
	{
		client.ConcatenateWBuffer(FormatedMessage("442", ":" + serv.getName(),
			 client.getNick() + " " + destChan.getName()
			 + " :You're not on that channel"), serv);
		return (false);
	}
	if (!destChan.isOps(senderCli.getNick()))
	{
		client.ConcatenateWBuffer(FormatedMessage("482", ":" + serv.getName(),
			 client.getNick() + " " + destChan.getName()
			 + " :You're not a channel operator"), serv);
		return (false);
	}
	if (invitedCli.isInChannel(destChan.getName()))
	{
	    client.ConcatenateWBuffer(FormatedMessage("443", ":" + serv.getName(),
	        client.getNick() + " " + invitedCli.getNick() + " " + destChan.getName()
			+ " :is already on channel"), serv);
	    return (false);
	}
	return (true);
}


std::vector<std::string>	getChangesToDo(std::vector<std::string>	&args)
{
	std::vector<std::string>	res;
	std::string 				&changeString 	= args[1];
	std::string					sign;

	for (size_t i = 0; i < changeString.size(); i++)
	{
		if (changeString[i] == '+')
		{
			sign = "+";
			continue;
		}
		if (changeString[i] == '-')
		{
			sign = "-";
			continue;
		}
		res.push_back(sign + std::string(1, changeString[i]));
	}
	return (res);
}

std::vector<std::string> getArgList(std::vector<std::string> &args)
{
	std::vector<std::string> res;

	for (size_t i = 2; i < args.size(); i++)
		res.push_back(args[i]);
	return (res);
}

void	processFlags(
	std::vector<std::string>	&changesToDo,
	std::vector<std::string>	&argList,
	ChanModes					&mode,
	ModeAction					&act,
	Channel						&destChan,
	Client						&client,
	char						&currentSign,
	std::string					&finalFlags,
	std::string					&finalArgs,
	Server						&serv
)
{
	for (size_t i = 0; i < changesToDo.size(); i++)
	{
		std::string potentialArg = argList.empty() ? "" : argList.front();
		size_t oldSize = argList.size();

		if (processModeChange(argList, changesToDo[i] , mode, act, destChan, client, serv))
		{
			if (currentSign != changesToDo[i][0])
			{
				currentSign = changesToDo[i][0];
				finalFlags += currentSign;
			}
			finalFlags += changesToDo[i][1];

			if (argList.size() < oldSize)
			{
				if (!finalArgs.empty())
					finalArgs += " ";
				finalArgs += potentialArg;
			}
		}
	}
}

std::string	CurrentHostname(Client& client)
{
	struct sockaddr_in		tmp;
	char					hostname[INET_ADDRSTRLEN];
	std::string				host;
	tmp = client.getClientInfos();
	inet_ntop(AF_INET, &(tmp.sin_addr), hostname, INET_ADDRSTRLEN);
	host = hostname;
	return (host);
}
