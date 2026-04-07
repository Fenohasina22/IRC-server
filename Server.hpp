/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:15 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/07 17:56:15 by fsamy-an         ###   ########.fr       */
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
#include <cstring>
#define	MSG_BUFFERSIZE 512

class Server
{
	private:
		struct sockaddr_in	_addr;
		int					_sockfd;
		std::vector<pollfd>	_vecPoll;


	public:
		Server();
		~Server();
		std::vector<pollfd>&	getVecPoll();
		int						getSockfd() const;
		sockaddr_in				getSocketstats() const;
		void					Initialize();
};



void			NewUserHandling(int sockfd, sockaddr_in& clientinfo, socklen_t&  csize,std::vector<pollfd>& vecpol);
std::string		BufferCleaning(char *buff);
void			Processmessage (int i, std::vector<pollfd>& vecpol);


#endif

