/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 15:10:46 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/12 15:24:42 by fsamy-an         ###   ########.fr       */
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
		//sendCodes(client.getFd(), "331", ":server", client.getNick() + " " + destChan.getName() + " :No topic is set");
		client.ConcatenateWBuffer(FormatedMessage("331", ":server", client.getNick() + " " + destChan.getName() + " :No topic is set"), serv);
	}
	else
	{

		//sendCodes(client.getFd(), "332", ":server", client.getNick() + " " + destChan.getName() + " :" + destChan.getTopic());
		client.ConcatenateWBuffer(FormatedMessage("332", ":server", client.getNick() + " " + destChan.getName() + " :" + destChan.getTopic()), serv);
	}
		// 332 nick #channel :topic text
		// 353 nick = #channel :@nick1 nick2 nick3
	std::string nameList = formNameList(destChan);
	mess += client.getNick();
	mess += " = "; // the = means “public channel”, just hardcode = for now
	mess += destChan.getName();
	mess += " :" + nameList;
	//sendCodes(client.getFd(), "353", ":server", mess);
	client.ConcatenateWBuffer(FormatedMessage("353", ":server", mess), serv);
	// 366 nick #channel :End of /NAMES list
	mess.clear();
	mess += client.getNick() + " ";
	mess += destChan.getName();
	mess += " :End of /NAMES list";
	//sendCodes(client.getFd(), "366", ":server", mess);
	client.ConcatenateWBuffer(FormatedMessage("366", ":server", mess), serv);

	// Unlike PRIVMSG, you do send JOIN back to the joining client
}
