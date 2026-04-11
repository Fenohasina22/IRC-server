/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:01:25 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 15:47:05 by mratsima         ###   ########.fr       */
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

int 			Client::getFd() const
{
	return (this->fd);
}

std::string	Client::getNick() const
{
	return (this->nickname);
}

std::string	Client::getUser() const
{
	return (this->username);
}

std::string	Client::getReal() const
{
	return (this->realname);
}

bool			Client::getNickState() const
{
	return (this->isNickOk);
}

bool			Client::getUserState() const
{
	return (this->isUserOk);
}

bool			Client::getPassState() const
{
	return (this->isPassOk);
}

bool			Client::isRegistered() const
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
	// check by channel name
	if (joinedChannels.count(c->getName()) > 0)
		return (true);
	return (false);
}


bool 	Client::isInChannel(std::string name) const
{
	if (name.empty())
		return (false);
	return (this->joinedChannels.count(name) > 0);
}

/*never call this*/
void	Client::addChannel(std::string chanName)
{
	this->joinedChannels.insert(chanName);
}

/*never call this*/
void	Client::removeChannel(std::string chanName)
{
	this->joinedChannels.erase(chanName);
}
