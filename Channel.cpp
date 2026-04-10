/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:09 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/10 15:31:13 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
: name("")
, topic("")
, members()
, ops()
{
}

Channel::Channel(std::string n, std::string t)
: name(n)
, topic(t)
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

const std::set<Client*>	&Channel::getMembers() const
{
	return (this->members);
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
	this->members.insert(c);
	c->addChannel(this);
}

void	Channel::removeClient(Client* c)
{
	this->members.erase(c);
	c->removeChannel(this);
}
