/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:00:12 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 12:49:13 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <set>
# include "Channel.hpp"

# ifndef	CRLF
# define CRLF "\r\n"
# endif

class Channel;

class Client
{
	private:
		int					fd;
		std::string 		nickname;
		std::string 		username;
		std::string 		realname;
		bool				isPassOk;
		bool				isNickOk;
		bool				isUserOk;
		bool				userIsRegistered;
		std::string			_readBuffer; /*Read buffer pollin to stock here per client*/
		std::string			_writeBuffer;/*Write buffer pollout to stock here per client*/
		std::set<Channel*>	joinedChannels;
		/*
			prevNick and CurrentNick
			to broadcast nick change;
		*/
	public:
		int 			getFd() const;
		std::string		getNick() const;
		std::string		getUser() const;
		std::string		getReal() const;
		std::string		getWriteBuffer() const;
		std::string		getReadBuffer() const;


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
		bool			isInChannel(Channel* c) const;
		void			addChannel(Channel *c);
		void			removeChannel(Channel *c);

		void			ConcatenateWBuffer(std::string str);
		void			ConcatenateRBuffer(std::string str);
		
		Client();
		Client&	operator=(const Client& c);
		~Client();
};

#endif
