/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:15 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/11 15:44:52 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <poll.h> // pour poll
# include <sys/socket.h> // pour socket , bind , etc
# include <netinet/in.h>
# include <unistd.h>
# include <vector>
# include "Client.hpp"
# include <cstring>
# include <errno.h>
# include "dispatch.hpp"
#include <signal.h>
# define	MSG_BUFFERSIZE 10

# ifndef	CRLF
# define CRLF "\r\n"
# endif


# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[1;36m"
# define RESET   "\033[0m"




class Server
{
	private:
		struct sockaddr_in		_addr;
		int						_sockfd;
		std::vector<pollfd>		_vecPoll;
		std::vector<Client>  	_allClients;
		std::vector<Channel>	_allChannels;
		std::string				_password;
		int						_port;


	public:
		Server();
		~Server();

		std::vector<pollfd>&	getVecPoll();
		int						getSockfd() const;
		sockaddr_in				getSocketstats() const;
		int						getPort() const;
		const std::string		getPass() const;
		void					Initialize();
		void 					setPass(std::string newPass);
		void					setPort(int newPort);
		std::vector<Client>		&getAllClients();
		std::vector<Channel>	&getAllChans();
		Client 					&findClient(int fd, bool &success);
		Client 					&findClient(std::string nick, bool &success);
		Channel 				&findChan(std::string name, bool &success);
		bool					NewUserHandling(sockaddr_in& clientinfo, socklen_t&  csize);
		void					Processmessage (int i);
		void					broadcast(std::string &mess, const Client &caster, const Channel &chan);
};

std::string		BufferCleaning(char *buff);

#endif

