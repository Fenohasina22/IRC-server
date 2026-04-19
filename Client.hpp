/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:00:12 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/19 08:34:59 by mratsima         ###   ########.fr       */
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
		int						_fd;
		std::string 			_nickname;
		std::string 			_username;
		std::string 			_realname;
		bool					_isPassOk;
		bool					_isNickOk;
		bool					_isUserOk;
		bool					_userIsRegistered;
		std::string				_readBuffer;
		std::string				_writeBuffer;
		std::set<std::string>	_joinedChannels;
		struct sockaddr_in		_clientinfos;
		bool					_pendingClose;

	public:
		int 				getFd() const;
		std::string			getNick() const;
		std::string			getUser() const;
		bool				getPendingClose() const;
		void				setPendingClose(bool state);
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

		void			ConcatenateWBuffer(std::string str, Server& server);
		void			ConcatenateRBuffer(std::string str);

		std::set<std::string>	getJoinedChannels() const;
		Client();
		Client&	operator=(const Client& c);
		~Client();
};

#endif
