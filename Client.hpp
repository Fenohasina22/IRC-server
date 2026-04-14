/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:00:12 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/14 08:18:22 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <set>
# include "Channel.hpp"
#include "Server.hpp"
# ifndef	CRLF
# define CRLF "\r\n"
# endif

class Channel;
class Server;
class Client
{
	private:
		int						fd;
		std::string 			nickname;
		std::string 			username;
		std::string 			realname;
		bool					isPassOk;
		bool					isNickOk;
		bool					isUserOk;
		bool					userIsRegistered;
		std::string				_readBuffer; /*Read buffer pollin to stock here per client*/
		std::string				_writeBuffer;/*Write buffer pollout to stock here per client*/
		std::set<std::string>	joinedChannels;
		struct sockaddr_in		_clientinfos;
		
		/*
			prevNick and CurrentNick
			to broadcast nick change;
		*/
	public:
		int 				getFd() const;
		std::string			getNick() const;
		std::string			getUser() const;
		std::string			getReal() const;
		std::string			getWriteBuffer() const;
		std::string			getReadBuffer() const;
		struct sockaddr_in	getClientInfos() ;


		bool			getNickState() const;
		bool			getUserState() const;
		bool			getPassState() const;
		bool			isRegistered() const;
		void 			setFd(int fd);
		void			setNick(std::string toSet);
		void			setUser(std::string toSet);
		void			setReal(std::string toSet);
		void			setWriteBuffer(std::string str);
		void			setReadBuffer(std::string str);
		void			setPassState(bool state);
		void			setNickState(bool state);
		void			setUserState(bool state);
		void			setRegistrationState(bool state);
		void			setClientInfos(struct sockaddr_in& infos);
		
		bool		 	isInChannel(std::string name) const;
		bool			isInChannel(Channel* c) const;
		void			addChannel(std::string chanName);
		void			removeChannel(std::string chanName);

		//void			ConcatenateWBuffer(std::string str);
		void			ConcatenateWBuffer(std::string str, Server& server);

		void			ConcatenateRBuffer(std::string str);
		
		std::set<std::string>	getJoinedChannels() const;
		Client();
		Client&	operator=(const Client& c);
		~Client();
};

#endif
