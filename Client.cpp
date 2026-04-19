/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:01:25 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/19 08:39:23 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>

Client::Client()
: _fd(-1)
, _nickname("")
, _username("")
, _realname("")
, _isPassOk(false)
, _isNickOk(false)
, _isUserOk(false)
, _userIsRegistered(false)
, _readBuffer("")
, _writeBuffer("")
, _pendingClose(false)

{
}

bool	Client::getPendingClose() const
{
	return this->_pendingClose;
}

void	Client::setPendingClose(bool state)
{
	this->_pendingClose = state;
}

Client::~Client()
{
}

int 			Client::getFd() const
{
	return (this->_fd);
}

std::string	Client::getNick() const
{
	return (this->_nickname);
}

std::string	Client::getUser() const
{
	return (this->_username);
}

std::string	Client::getReal() const
{
	return (this->_realname);
}

bool			Client::getNickState() const
{
	return (this->_isNickOk);
}

bool			Client::getUserState() const
{
	return (this->_isUserOk);
}

bool			Client::getPassState() const
{
	return (this->_isPassOk);
}

struct sockaddr_in	Client::getClientInfos()
{
	return (this->_clientinfos);
}

bool			Client::isRegistered() const
{
	return (this->_userIsRegistered);
}

std::set<std::string>	Client::getJoinedChannels() const
{
	return (this->_joinedChannels);
}

void			Client::setClientInfos(struct sockaddr_in& infos)
{
	this->_clientinfos.sin_addr = infos.sin_addr;
	this->_clientinfos.sin_family = infos.sin_family;
	this->_clientinfos.sin_port = infos.sin_port;
}


void 				Client::setFd(int fd)
{
	this->_fd  = fd;
}

void				Client::setNick(std::string toSet)
{
	this->_nickname = toSet;
}

void				Client::setUser(std::string toSet)
{
	this->_username = toSet;
}

void				Client::setReal(std::string toSet)
{
	this->_realname = toSet;
}

void				Client::setPassState(bool state)
{
	this->_isPassOk = state;
}

void				Client::setNickState(bool state)
{
	this->_isNickOk = state;
}

void				Client::setUserState(bool state)
{
	this->_isUserOk = state;
}

void				Client::setRegistrationState(bool state)
{
	this->_userIsRegistered = state;
}


bool 	Client::isInChannel(Channel* c) const
{
	if (!c)
		return (false);
	std::string chanName = toLower(c->getName());
	for (std::set<std::string>::const_iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
	{
		std::string joined = toLower(*it);
		if (joined == chanName)
			return true;
	}
	return false;
}


bool 	Client::isInChannel(std::string name) const
{
	if (name.empty())
		return (false);
	std::string lowerName = toLower(name);
	for (std::set<std::string>::const_iterator it = _joinedChannels.begin(); it != _joinedChannels.end(); ++it)
	{
		std::string joined = toLower(*it);
		if (joined == lowerName)
			return true;
	}
	return false;
}

/*never call this*/
void	Client::addChannel(std::string chanName)
{
	this->_joinedChannels.insert(chanName);
}

/*never call this*/
void	Client::removeChannel(std::string chanName)
{
	this->_joinedChannels.erase(chanName);
}

std::string	Client::getWriteBuffer() const
{
	return (this->_writeBuffer);
}

std::string	Client::getReadBuffer() const
{
	return (this->_readBuffer);
}

void	Client::setReadBuffer(std::string str)
{
	this->_readBuffer = str;
}

void	Client::setWriteBuffer(std::string str)
{
	this->_writeBuffer = str;
}

void	Client::ConcatenateWBuffer(std::string str, Server& server)
{
	bool a = false;
	pollfd&	tmp = server.findElementByfd(this->_fd, a);

	if (a == false)
	{
		std::cout << "NO SUCH CLIENT" << std::endl;
		return ;
	}
	this->_writeBuffer += str;
	tmp.events |= POLLOUT;
}

void	Client::ConcatenateRBuffer(std::string str)
{
	this->_readBuffer += str;
}

Client&	Client::operator=(const Client& c)
{
    if (this != &c)
    {
        this->_fd = c._fd;
        this->_nickname = c._nickname;
        this->_username = c._username;
        this->_realname = c._realname;
        this->_isPassOk = c._isPassOk;
        this->_isNickOk = c._isNickOk;
        this->_isUserOk = c._isUserOk;
        this->_userIsRegistered = c._userIsRegistered;
        this->_readBuffer = c._readBuffer;
        this->_writeBuffer = c._writeBuffer;
        this->_joinedChannels = c._joinedChannels;
		this->_pendingClose = c._pendingClose;
    }
    return (*this);
}


