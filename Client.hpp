/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:00:12 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 18:05:19 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <set>
# include "Channel.hpp"

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
		std::string			readBuffer; /*no idea what it does yet*/
		std::string			writeBuffer;/*no idea what it does yet*/
		std::set<Channel*>	joinedChannels;
		/*
			prevNick and CurrentNick
			to broadcast nick change;
		*/
	public:
		const int 			getFd() const;
		const std::string	getNick() const;
		const std::string	getUser() const;
		const std::string	getReal() const;
		const bool			getNickState() const;
		const bool			getUserState() const;
		const bool			getPassState() const;
		const bool			isRegistered() const;
		void 				setFd(int fd);
		void				setNick(std::string toSet);
		void				setUser(std::string toSet);
		void				setReal(std::string toSet);
		void				setPassState(bool state);
		void				setNickState(bool state);
		void				setUserState(bool state);
		void				setRegistrationState(bool state);
		bool				isInChannel(Channel* c) const;
		void				addChannel(Channel *c);
		void				removeChannel(Channel *c);

		Client();
		~Client();
};

#endif
