/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:09 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/14 12:34:51 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel()
: name("")
, topic("")
, members()
, ops()
, pass("")
, maxUser(512)
, _isInviteOnly(false)
, _isTopicLocked(false)
, _isPassRequired(false)
, _isUserLimitEnabled(false)
{
}

Channel::Channel(std::string n, std::string t)
: name(n)
, topic(t)
, members()
, ops()
, pass("")
, maxUser(512)
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
	return (this->name);
}

const std::string	Channel::getTopic() const
{
	return (this->topic);
}

const std::set<std::string> &Channel::getMembers() const
{
	return (this->members);
}

const std::string	Channel::getPass() const
{
	return (this->pass);
}

void	Channel::setPass(const std::string &p)
{
	this->pass = p;
}

int	Channel::getMaxUser() const
{
	return (this->maxUser);
}

void	Channel::setMaxUser(int v)
{
	this->maxUser = v;
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
	this->name = toSet;
}

void	Channel::setTopic(const std::string &toSet)
{
	this->topic = toSet;
}

bool	Channel::operator==(const Channel &other)
{
	if (this->name == other.getName())
		return (true);
	return (false);
}

void	Channel::addClient(Client* c)
{
	this->members.insert(c->getNick());
	c->addChannel(this->name);
}

void	Channel::addOperator(Client* c)
{
	this->ops.insert(c->getNick());
}

void	Channel::addInvited(Client* c)
{
	this->invited.insert(c->getNick());
}

void	Channel::removeClient(Client* c)
{
	this->members.erase(c->getNick());
	c->removeChannel(this->name);
}

void	Channel::removeOperator(Client* c)
{
	this->ops.erase(c->getNick());
}

void	Channel::addFlag(std::string flag)
{
	this->flags.insert(flag);
}

void	Channel::removeFlag(std::string flag)
{
	if (!flags.empty())
		this->flags.erase(flag);
}

bool	Channel::isOps(Client &c)
{
	return (this->ops.count(c.getNick()) > 0);
}

bool	Channel::isOps(const std::string &nick) const
{
	return (this->ops.count(nick) > 0);
}

bool	Channel::isInvited(const std::string &nick) const
{
	return (this->ops.count(nick) > 0);
}

std::string	Channel::flagsToStr()
{
	std::string					res;
	std::vector<std::string>	args;

	if (!this->flags.empty())
		res += "+";
	if (this->isInviteOnly())
		res += "i";
	if (this->isTopicLocked())
		res += "t";
	if (this->isPassRequired())
	{
		res += "k";
		args.push_back(this->pass);
	}
	if (this->isUserLimitEnabled())
	{
		std::stringstream	ss;
		std::string 		strMax;

		ss << this->maxUser;
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
