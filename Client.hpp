/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:00:12 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 12:57:09 by mratsima         ###   ########.fr       */
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
		bool				isRegistered;
		std::string			readBuffer; /*no idea what it does yet*/
		std::string			writeBuffer;/*no idea what it does yet*/
		std::set<Channel*>	joinedChannels;
		/*
			prevNick and CurrentNick
			to broadcast nick change;
		*/
	public:
		Client();
		~Client();
};

#endif
