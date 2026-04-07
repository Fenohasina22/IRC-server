/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:01:25 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 16:44:52 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>

Client::Client()
{
}

Client::~Client()
{
}

const int 			Client::getFd() const
{
	return (this->fd);
}

const std::string	Client::getNick() const
{
	return (this->nickname);
}

const std::string	Client::getUser() const
{
	return (this->username);
}

const std::string	Client::getReal() const
{
	return (this->realname);
}


const bool			Client::isRegistered() const
{
	if (this->isPassOk && this->isNickOk && this->isUserOk)
		return (true);
	return (false);
}

void 				Client::setFd(int fd)
{
	this->fd  = fd;
}

void				Client::setNick(std::string toSet)
{
	this->nickname = toSet;
}

void				Client::setUser(std::string toSet)
{
	this->username = toSet;
}

void				Client::setReal(std::string toSet)
{
	this->realname = toSet;
}

bool 	Client::isInChannel(Channel* c) const
{
	if (!c)
		return (false);
	if (std::find(this->joinedChannels.begin(), this->joinedChannels.end(), c) != this->joinedChannels.end())
		return (true);
	return (false);
}

/*never call this*/
void	Client::addChannel(Channel *c)
{
	this->joinedChannels.insert(c);
}

/*never call this*/
void	Client::removeChannel(Channel *c)
{
	this->joinedChannels.erase(c);
}
