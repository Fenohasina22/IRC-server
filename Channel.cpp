/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:09 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 11:18:56 by mratsima         ###   ########.fr       */
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

void	Channel::addOperator(Client* c)
{
	this->ops.insert(c);
}

void	Channel::removeClient(Client* c)
{
	this->members.erase(c);
	c->removeChannel(this);
}

bool	Channel::isOps(Client &c)
{
	std::set<Client*>::iterator MitBegin	= this->members.begin();
	std::set<Client*>::iterator MitEnd 		= this->members.end();
	std::set<Client*>::iterator OpitBegin 	= this->ops.begin();
	std::set<Client*>::iterator OpitEnd 	= this->ops.end();

	for (std::set<Client*>::iterator it = MitBegin; it != MitEnd ; it++)
	{
		Client &curr = (**it);
		for (std::set<Client*>::iterator i = OpitBegin; i != OpitEnd ; i++)
		{
			if ((*i)->getNick() == curr.getNick())
				return (true);
		}
	}
	return (false);
}
