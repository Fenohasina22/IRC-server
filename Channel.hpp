/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 11:25:05 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/10 15:29:37 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include "Client.hpp"

# ifndef	CRLF
# define CRLF "\r\n"
# endif




class Client;

class Channel
{
	private:
		std::string 				name;
		std::string					topic;
		std::set<Client*>			members;
		std::set<Client*>			ops;

	public:
		const std::string			getName() const;
		const std::string			getTopic() const;
		const std::set<Client *>	&getMembers() const;
		void						setName(const std::string &toSet);
		void						setTopic(const std::string &toSet);

		void						addClient(Client* c);
		void						removeClient(Client* c);
		bool						operator==(const Channel &other);

		Channel();
		Channel(std::string name, std::string topic);
		~Channel();
};

#endif
