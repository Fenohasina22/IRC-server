/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:05 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 16:23:14 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include <set>
# include <string>

# ifndef	CRLF
# define CRLF "\r\n"
# endif




class Client;

class Channel
{
	private:
		std::string 				_name;
		std::string					_topic;
		std::set<std::string>		_members;
		std::set<std::string>		_ops;
		std::set<std::string>		_invited;
		std::set<std::string>		_flags;
		std::string					_pass;
		int							_maxUser;
		bool						_isInviteOnly;
		bool						_isTopicLocked;
		bool						_isPassRequired;
		bool						_isUserLimitEnabled;

	public:
		const std::string			getName() const;
		const std::string			getTopic() const;
		std::set<std::string>		&getMembers();
		std::set<std::string>		&getOps();
		std::set<std::string>		&getInvited();
		const std::string			getPass() const;
		void						setPass(const std::string &p);
		int							getMaxUser() const;
		void						setMaxUser(int v);
		bool						isInviteOnly() const;
		void						setInviteOnly(bool v);
		bool						isTopicLocked() const;
		void						setTopicLocked(bool v);
		bool						isPassRequired() const;
		void						setPassRequired(bool v);
		bool						isUserLimitEnabled() const;
		void						setUserLimitEnabled(bool v);
		bool						isOps(const std::string &nick) const;
		bool						isInvited(const std::string &nick) const;
		void						setName(const std::string &toSet);
		void						setTopic(const std::string &toSet);
		void						addFlag(std::string flag);
		void						removeFlag(std::string flag);

		void						addClient(Client* c);
		void						addOperator(Client* c);
		void						addInvited(Client* c);
		void						removeClient(Client* c);
		void						removeOperator(Client* c);
		void						removeInvited(Client* c);
		bool						isOps(Client &c);
		bool						operator==(const Channel &other);
		std::string					flagsToStr();

		Channel();
		Channel(std::string name, std::string topic);
		~Channel();
};

#endif
