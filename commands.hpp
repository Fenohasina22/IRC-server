/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 16:49:06 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/19 06:57:35 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP
# include <iostream>
# include "parser.hpp"
# include "Client.hpp"
# include <sys/socket.h>
# include <arpa/inet.h>
# include "Server.hpp"
# include <sstream>

# ifndef	CRLF
# define CRLF "\r\n"
# endif

class Server;

typedef enum ChanModes
{
			i,
			o,
			t,
			k,
			l,
			unknown
}			ChanModes;

typedef enum ModeAction
{
			ADD,
			REMOVE,
			NO_ACTION
}			ModeAction;

/************************************COMMANDS*********************************/
bool	capCmd(Client &client, iRCMessage &mess, Server& serv);
bool	passCmd(Client &client, iRCMessage &mess, Server& serv, bool& validPass);
bool	nickCmd(Client &client, iRCMessage &mess, Server& serv);
bool	userCmd(Client &client, iRCMessage &mess, Server& serv);
bool	pongCmd(Client &client, iRCMessage &mess, Server& serv);
bool	privmsgCmd(Client &client, iRCMessage &mess, Server &Serv);
bool	joinCmd(Client &client, iRCMessage &mess, Server &Serv);
bool	partCmd(Client &client, iRCMessage &mess, Server &serv);
bool	topicCmd(Client &client, iRCMessage &mess, Server &serv);
bool	kickCmd(Client &client,iRCMessage &mess,Server &serv);
bool	inviteCmd(Client &client,iRCMessage &mess,Server &serv);
bool	quitCmd(iRCMessage& mess, Client& client, Server& serv);
bool	modeCmd(Client &client,iRCMessage &mess,Server &serv);

/************************************UTILS************************************/
void 						tryRegistration(Client &client, Server& serv);
bool	 					chanExists(const std::string &chanName, Server &serv);
std::string					formMess(const Client	&sender,const Client &destCli
							,const iRCMessage &mess);
std::string					formChanMess(const Client	&sender,const Channel &destChan,
							const iRCMessage &mess);
bool						isNicknameInUse(Server &serv, Client &client, std::string &newNick);
void						sendChannelState(Client &client, Channel &destChan, Server& serv);
ChanModes					strToMode(std::string strMode, ModeAction &action);
void						doIflag(Channel &destChan, ModeAction &act);
void						doTflag(Channel &destChan, ModeAction &act);
bool						doKflag(Channel &destChan, ModeAction &act, std::vector<std::string> &args);
bool						doLflag(Channel &destChan, ModeAction &act, std::vector<std::string> &args);
int							doOflag(Channel &destChan, ModeAction &act, std::vector<std::string> &args
							, Client &client, Server &serv);
void						DeleteVecElement(std::vector<pollfd>& vec, int i);
void						DeleteVecElementClient(std::vector<Client>& vec, int fd);
bool						getNeighbors(Client &client, Server &serv, std::set<std::string> &membersToNotify);
bool						notifyNeighbors(Client &client, Server &serv, std::string &newNick);
void						CleanUp(Server& serv, int saveFd);
bool						IsValidNick(std::string nick, Client&  client, Server& serv);
void						privmsgToChan(Client &sender, Channel &destChan, Server &serv,iRCMessage &mess,
							std::string &messageOutput);
bool						checkChanRestrictions(Client &client, Server &serv, iRCMessage &mess, Channel *destChan);
void						broadcastJoin(std::string &broadcastMess, Client &client, Server &serv, Channel *destChan);
bool						processTopicCommand(bool &displayTopic,bool &changeTopic,bool &resetTopic,Client &client,
							iRCMessage &mess,Server &serv,Channel &destChan);
void						updateTopic(bool &changeTopic,bool &resetTopic,iRCMessage &mess,Channel &destChan,
							std::string &broadcastMess,Client &client, Server &serv);
bool						checkChannelAccess(bool &foundChan, Client &client, iRCMessage &mess, Server &serv,
							Client &destCli, Channel &destChan, bool &foundCli);
void						broadCastKick(std::string &broadcastMess,Client &client,Client &destCli,
							Channel &destChan,iRCMessage 	&mess,Server &serv);
bool						validateChannelModeAccess(Server &serv, Client &client, iRCMessage &mess);
void						broacastModeChange(Client &client, Channel &destChan, const std::string &finalModeStr, Server &serv);
bool						processModeChange(std::vector<std::string> &args, std::string	&modeChange, ChanModes &mode
							,ModeAction &act,Channel	&destChan,Client 	&client,Server		&serv);
bool						validateInvite(bool  &foundCli,Client &client,bool  &foundChan,
							Client  &senderCli,iRCMessage 	&mess,Channel &destChan,
							Server  &serv,Client  &invitedCli);
bool						updateChannels(Client &client, Server &serv, std::string &newNick);
std::vector<std::string>	getChangesToDo(std::vector<std::string>	&args);
std::vector<std::string>	getArgList(std::vector<std::string> &args);
void						processFlags(std::vector<std::string>&changesToDo,std::vector<std::string>&argList,
							ChanModes&mode,ModeAction&act,Channel&destChan,Client&client,char&currentSign,
							std::string&finalFlags,std::string&finalArgs,Server&serv);
std::string					CurrentHostname(Client& client);
std::string					toLower(const std::string& str);

#endif
