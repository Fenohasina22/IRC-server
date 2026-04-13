/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:05 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/13 09:12:14 by mratsima         ###   ########.fr       */
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
		std::string 				name;
		std::string					topic;
		std::set<std::string>		members;
		std::set<std::string>		ops;
		std::set<std::string>		invited;

	public:
		const std::string			getName() const;
		const std::string			getTopic() const;
		const std::set<std::string>	&getMembers() const;
		bool						isOps(const std::string &nick) const;
		void						setName(const std::string &toSet);
		void						setTopic(const std::string &toSet);

		void						addClient(Client* c);
		void						addOperator(Client* c);
		void						addInvited(Client* c);
		void						removeClient(Client* c);
		bool						isOps(Client &c);
		bool						operator==(const Channel &other);

		Channel();
		Channel(std::string name, std::string topic);
		~Channel();
};

#endif
