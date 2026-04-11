/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:48:57 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 15:10:34 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void 	tryRegistration(Client &client)
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
		//send(client.getFd(), CRLF , 2, 0);
		//send(client.getFd(), msg001.c_str(), msg001.size(), 0);
		//send(client.getFd(), msg002.c_str(), msg002.size(), 0);
		//send(client.getFd(), msg003.c_str(), msg003.size(), 0);
		//send(client.getFd(), msg004.c_str(), msg004.size(), 0);
		// send(client.getFd(), all.c_str(), all.size(), 0);
		client.ConcatenateWBuffer(all);
	}
}

bool	passCmd(Client &client, iRCMessage &mess, Server &serv)
{
	// std::cout << "inpasscmd" <<std::endl;
    if (client.isRegistered())
    {
		//sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		client.ConcatenateWBuffer(FormatedMessage("462", ":server", ":You may not reregister"));
		return (false);
	}
    if (mess.args.empty())
    {
		//sendCodes(client.getFd(), "461", ":server", "* PASS :Not enough parameters");
		client.ConcatenateWBuffer(FormatedMessage("461", ":server", "* PASS :Not enough parameters"));

		return (false);
	}
    if (mess.args[0] != serv.getPass())
    {
		//sendCodes(client.getFd(), "464", ":server", ":Password incorrect");
		client.ConcatenateWBuffer(FormatedMessage("464", ":server", ":Password incorrect"));
		
		return (false);
	}
    client.setPassState(true);
	// std::cout << "PASS ok" << std::endl;
	return (true);
}

bool	nickCmd(Client &client, iRCMessage &mess, Server &serv)
{
	std::string newNick;

	if (mess.args.empty())
    {
		//sendCodes(client.getFd(), "431", ":server", ":No nickname specified");
		client.ConcatenateWBuffer(FormatedMessage("431", ":server", ":No nickname specified"));
		return (false);
	}
	newNick = mess.args[0];


	/*---------------------------------fix this ----------------------------------------------*/
	for (unsigned int i = 0; i < serv.getAllClients().size(); i++)
	{
		if (serv.getAllClients()[i].getNick() == newNick)
		{
			if (!client.isRegistered())
				sendCodes(client.getFd(), "433", ":server", "* " + newNick + " :Nickname is already in use");
			else
				sendCodes(client.getFd(), "433", ":server", client.getNick() + " " + newNick + " :Nickname is already in use");
			return (false);
		}
	}
	/*-------------------------------------------------------------------------------*/
	std::cout <<"new nick = " << newNick << std::endl;
	client.setNick(newNick);
	client.setNickState(true);
	tryRegistration(client); // concatenate
	return (true);
}

bool	userCmd(Client &client, iRCMessage &mess)
{
	if (client.isRegistered())
    {
		//sendCodes(client.getFd(), "462", ":server", ":You may not reregister");
		client.ConcatenateWBuffer(FormatedMessage("462", ":server", ":You may not reregister"));
		return (false);
	}
    if (mess.args.size() < 4)
    {
		//sendCodes(client.getFd(), "461" , ":server", "* USER :Not enough parameters");
		printiRCMESS(mess);
		client.ConcatenateWBuffer(FormatedMessage("461" , ":server", "* USER :Not enough parameters"));
		
		return (false);
	}
    client.setUser(mess.args[0]);
    client.setReal(mess.args[3]);
    client.setUserState(true);
    tryRegistration(client);
	return (true);
}

bool	capCmd(Client &client)
{
	if (client.isRegistered())
		return (false);
    //send(client.getFd(), ":server CAP * LS :\r\n", 21, 0);
    //send(client.getFd(), ":server CAP * END\r\n", 20, 0);
	std::string		cap;

	client.ConcatenateWBuffer(":server CAP * LS :\r\n");
	if (client.isRegistered())
		client.ConcatenateWBuffer(":server CAP * END\r\n");
    std::cout << GREEN << "U got CAP HERE" << RESET << std::endl;

	return (true);
}

bool	pongCmd(Client &client, iRCMessage &mess)
{
	std::string pongstr;

	if (mess.args.empty())
		pongstr = CRLF;
	else
		pongstr = mess.args[0] + CRLF;
	//send(client.getFd(), pongstr.c_str(), pongstr.size(), 0);
	client.ConcatenateWBuffer(pongstr);
	return (true);
}

bool	privmsgCmd(Client &client, iRCMessage &mess, Server &serv)
{
	/*need to add option to send to channels too*/
	bool			foundClient 	= false;
	Client			&destination 	= serv.findClient(mess.args[0], foundClient);
	const Client 	&sender = client;
	std::string 	messageOutput;
	if (!foundClient)
	{
		// :<server> 401 <client> <target> :No such nick/channel\r\n
		//sendCodes(client.getFd(), "401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick/channel");
		client.ConcatenateWBuffer(FormatedMessage("401", ":server", client.getNick() + " " + mess.args[0] + " :No such nick/channel"));
		return (false);
	}
	/*
	: <nick> PRIVMSG <dnick> <message> CRLF
	*/
	messageOutput += ":" + sender.getNick();
	messageOutput += " PRIVMSG ";
	messageOutput += destination.getNick() + " ";
	messageOutput += mess.args[1];
	messageOutput += CRLF;
	//send(destination.getFd(), messageOutput.c_str(), messageOutput.size(), 0);
	std::cout << messageOutput << std::endl;
	client.ConcatenateWBuffer(messageOutput);
	return (true);
}
