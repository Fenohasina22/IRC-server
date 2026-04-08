/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:15 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/08 06:47:32 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <poll.h> // pour poll
#include <sys/socket.h> // pour socket , bind , etc
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include "Client.hpp"
#include <cstring>
#include <errno.h>
#include "mratsima/dispatch.hpp"
#define	MSG_BUFFERSIZE 1024

class Server
{
	private:
		struct sockaddr_in		_addr;
		int						_sockfd;
		std::vector<pollfd>		_vecPoll;
		std::vector<Client>  	_allClients;
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
		std::vector<Client>&	getAllClients();
		bool					NewUserHandling(sockaddr_in& clientinfo, socklen_t&  csize);
		void					Processmessage (int i);
};

std::string		BufferCleaning(char *buff);

#endif

