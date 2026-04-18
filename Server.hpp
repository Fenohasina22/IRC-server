/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:15 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/18 16:56:36 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <vector>
# include "Client.hpp"
# include <cstring>
# include "dispatch.hpp"
#include <signal.h>
# define	MSG_BUFFERSIZE 1024

# ifndef	CRLF
# define CRLF "\r\n"
# endif


# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[1;36m"
# define RESET   "\033[0m"


extern std::vector<int> AllFds;

class Server
{
	private:
		struct sockaddr_in		_addr;
		int						_sockfd;
		std::vector<pollfd>		_vecPoll;
		std::vector<Client>  	_allClients;
		std::vector<Client>		_trueClients;
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
		std::vector<Client>&	getTrueClients();
		int						Initialize();
		void 					setPass(std::string newPass);
		void					setPort(int newPort);
		std::vector<Client>		&getAllClients();
		std::vector<Channel>	&getAllChans();
		Client 					&findClient(int fd, bool &success);
		Client&					findTrueClient(int fd, bool	&success);
		Client&					findTrueClient(std::string nick, bool &success);
		Client 					&findClient(std::string nick, bool &success);
		Channel 				&findChan(std::string name, bool &success);
		bool					NewUserHandling(sockaddr_in& clientinfo, socklen_t&  csize);
		void					Processmessage (int i);
		void					deleteChan(std::string &chanName);
		void					broadcastWithoutChan(std::string &mess, const Client &caster, std::set<std::string> members, Server& serv);
		void					broadcast(std::string &mess, const Client &caster, Channel &chan, Server& serv);
		pollfd&					findElementByfd(int fd, bool& a);
		void					removeClientByFd(int fd);

	};


#endif

