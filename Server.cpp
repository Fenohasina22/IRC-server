/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:41:51 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/07 18:10:40 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


/*SERVER*/
Server::Server()
{
}

Server::~Server()
{
}

std::vector<pollfd>&	Server::getVecPoll()
{
	return (this->_vecPoll);
}
int			Server::getSockfd() const
{
	return (this->_sockfd);
}

sockaddr_in	Server::getSocketstats() const
{
	return (this->_addr);
}

void	Server::Initialize()
{
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(6667);
	this->_addr.sin_addr.s_addr = INADDR_ANY; 
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a socket IPv4, TCP
	// bind adding info to the socket
	if (bind(this->_sockfd, (struct sockaddr *)&(this->_addr), sizeof(this->_addr)) == 0) 
		std::cout << "Binding successfull" << std::endl;
	else
		std::cout << "binding failed" << std::endl;	
	if (listen(this->_sockfd, 10) == 0) // socket is in listen mode 10 en file d'attente
		std::cout << "Listen successful"<<  std::endl;
	else
		std::cout << "Listen failed" << std::endl;
}

void	NewUserHandling(int sockfd, sockaddr_in& clientinfo, 
							socklen_t&  csize,std::vector<pollfd>& vecpol)
{
	struct pollfd tmp;

	tmp.fd = accept(sockfd, (sockaddr *)&clientinfo, &csize);
	tmp.events = POLLIN;
	std::cout << "New user connected from port : " << clientinfo.sin_port << std::endl;
	vecpol.push_back(tmp);
}

std::string		BufferCleaning(char *buff)
{
	std::string	result;

	int	i;

	i = 0;
	while (buff[i + 1])
	{
		result += buff[i];
		if (buff[i] == '\r' && buff[i + 1] == '\n')
		{
			result += "\r\n";
			return (result);
		}
		i++;
	}
	return (result);
}


void	Processmessage (int i, std::vector<pollfd>& vecpol)
{
	char				buff[MSG_BUFFERSIZE + 1];
	int					retval;
	
	memset (buff, 0, MSG_BUFFERSIZE);
	retval = recv(vecpol[i].fd, buff, MSG_BUFFERSIZE, 0);
	if (retval == -1)
	{
		std::cout << "Recv error" << std::endl;
	}
	std::cout << buff << std::endl; //still print undefined char
}

