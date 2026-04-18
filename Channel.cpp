/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:09 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 19:32:48 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

std::string toLower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = std::tolower(result[i]);
    }
    return result;
}

Channel::Channel()
: _name("")
, _topic("")
, _members()
, _ops()
, _pass("")
, _maxUser(512)
, _isInviteOnly(false)
, _isTopicLocked(false)
, _isPassRequired(false)
, _isUserLimitEnabled(false)
{
}

Channel::Channel(std::string n, std::string t)
: _name(n)
, _topic(t)
, _members()
, _ops()
, _pass("")
, _maxUser(512)
, _isInviteOnly(false)
, _isTopicLocked(false)
, _isPassRequired(false)
, _isUserLimitEnabled(false)
{
}

Channel::~Channel()
{
}

const std::string	Channel::getName() const
{
	return (this->_name);
}

const std::string	Channel::getTopic() const
{
	return (this->_topic);
}

std::set<std::string> &Channel::getMembers()
{
	return (this->_members);
}

std::set<std::string> &Channel::getOps()
{
	return (this->_ops);
}

std::set<std::string> &Channel::getInvited()
{
	return (this->_invited);
}

const std::string	Channel::getPass() const
{
	return (this->_pass);
}

void	Channel::setPass(const std::string &p)
{
	this->_pass = p;
}

int	Channel::getMaxUser() const
{
	return (this->_maxUser);
}

void	Channel::setMaxUser(int v)
{
	this->_maxUser = v;
}

bool	Channel::isInviteOnly() const
{
	return (this->_isInviteOnly);
}

void	Channel::setInviteOnly(bool v)
{
	this->_isInviteOnly = v;
}

bool	Channel::isTopicLocked() const
{
	return (this->_isTopicLocked);
}

void	Channel::setTopicLocked(bool v)
{
	this->_isTopicLocked = v;
}

bool	Channel::isPassRequired() const
{
	return (this->_isPassRequired);
}

void	Channel::setPassRequired(bool v)
{
	this->_isPassRequired = v;
}

bool	Channel::isUserLimitEnabled() const
{
	return (this->_isUserLimitEnabled);
}

void	Channel::setUserLimitEnabled(bool v)
{
	this->_isUserLimitEnabled = v;
}

void	Channel::setName(const std::string &toSet)
{
	this->_name = toSet;
}

void	Channel::setTopic(const std::string &toSet)
{
	this->_topic = toSet;
}

bool	Channel::operator==(const Channel &other)
{
	if (this->_name == other.getName())
		return (true);
	return (false);
}

void	Channel::addClient(Client* c)
{
	std::string nick = c->getNick();
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower);
	this->_members.insert(nick);
	c->addChannel(this->_name);
}

void	Channel::addOperator(Client* c)
{
	std::string nick = c->getNick();
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower);
	this->_ops.insert(nick);
}

void	Channel::addInvited(Client* c)
{
	std::string nick = c->getNick();
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower);
	this->_invited.insert(nick);
}

void	Channel::removeClient(Client* c)
{
	this->_members.erase(c->getNick());
	if (this->isOps(c->getNick()))
		this->removeOperator(c);
	c->removeChannel(this->_name);
}

void	Channel::removeOperator(Client* c)
{
	this->_ops.erase(c->getNick());
}

void	Channel::addFlag(std::string flag)
{
	this->_flags.insert(flag);
}

void	Channel::removeFlag(std::string flag)
{
	if (!_flags.empty())
		this->_flags.erase(flag);
}

void	Channel::removeInvited(Client *c)
{
	if (!_invited.empty())
		this->_invited.erase(c->getNick());
}

bool	Channel::isOps(Client &c)
{
	std::string nick = toLower(c.getNick());
	for (std::set<std::string>::iterator it = _ops.begin(); it != _ops.end(); ++it)
	{
		std::string opNick = toLower(*it);
		if (opNick == nick)
			return true;
	}
	return false;
}

bool	Channel::isOps(const std::string &nick) const
{
	std::string lowerNick = nick;
	std::transform(lowerNick.begin(), lowerNick.end(), lowerNick.begin(), ::tolower);
	for (std::set<std::string>::const_iterator it = _ops.begin(); it != _ops.end(); ++it)
	{
		std::string opNick = *it;
		std::transform(opNick.begin(), opNick.end(), opNick.begin(), ::tolower);
		if (opNick == lowerNick)
			return true;
	}
	return false;
}

bool	Channel::isInvited(const std::string &nick) const
{
	std::string lowerNick = toLower(nick);
	for (std::set<std::string>::const_iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		std::string invNick = toLower(*it);
		if (invNick == lowerNick)
			return true;
	}
	return false;
}

std::string	Channel::flagsToStr()
{
	std::string					res;
	std::vector<std::string>	args;


	res += "+";
	if (this->isInviteOnly())
		res += "i";
	if (this->isTopicLocked())
		res += "t";
	if (this->isPassRequired())
	{
		res += "k";
		args.push_back(this->_pass);
	}
	if (this->isUserLimitEnabled())
	{
		std::stringstream	ss;
		std::string 		strMax;

		ss << this->_maxUser;
		ss >> strMax;

		res += "l";
		args.push_back(strMax);
	}
	res += " ";
	for (size_t i = 0; i < args.size(); i++)
	{
		res += args[i];
		if (i != args.size() - 1)
			res += " ";
	}
	return (res);
}
