/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:01:25 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 18:49:09 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>

Client::Client()
: fd(-1)
, nickname("")
, username("")
, realname("")
, isPassOk(false)
, isNickOk(false)
, isUserOk(false)
, userIsRegistered(false)
, readBuffer("")
, writeBuffer("")

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

const bool			Client::getNickState() const
{
	return (this->isNickOk);
}

const bool			Client::getUserState() const
{
	return (this->isUserOk);
}

const bool			Client::getPassState() const
{
	return (this->isPassOk);
}

const bool			Client::isRegistered() const
{
	return (this->userIsRegistered);
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

void				Client::setPassState(bool state)
{
	this->isPassOk = state;
}

void				Client::setNickState(bool state)
{
	this->isNickOk = state;
}

void				Client::setUserState(bool state)
{
	this->isUserOk = state;
}

void				Client::setRegistrationState(bool state)
{
	this->userIsRegistered = state;
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
