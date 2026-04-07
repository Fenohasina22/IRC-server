/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:41:51 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/07 19:53:27 by fsamy-an         ###   ########.fr       */
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
std::vector<Client>& Server::getAllClients()
{
	return (this->_allClients);
}
int			Server::getSockfd() const
{
	return (this->_sockfd);
}

void 					Server::setPass(std::string newPass)
{
	this->_password = newPass;
}

int						Server::setPort(int newPort)
{
	this->_port = newPort;
}

const int						Server::getPort() const
{
	return (this->_port);
}

const std::string				Server::getPass() const
{
	return (this->_password);
}

sockaddr_in	Server::getSocketstats() const
{
	return (this->_addr);
}

void	Server::Initialize()
{
	int opt;

	this->_addr.sin_family = AF_INET;
	this->_addr.sin_port = htons(6667);
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a socket IPv4, TCP
	// bind adding info to the socket
	opt = 1;
	if (setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cout << "setsockopt error" << std::endl;
		/*err handling here*/
		return ;
	}
	if (bind(this->_sockfd, (struct sockaddr *)&(this->_addr), sizeof(this->_addr)) == 0) 
		std::cout << "Binding successfull" << std::endl;
	else
		std::cout << "binding failed" << std::endl;
	if (listen(this->_sockfd, 10) == 0) // socket is in listen mode 10 en file d'attente
		std::cout << "Listen successful"<<  std::endl;
	else
		std::cout << "Listen failed" << std::endl;
}

bool	Server::NewUserHandling(sockaddr_in& clientinfo, socklen_t&  csize, int i)
{
	struct pollfd tmp;
	Client	client;

	tmp.fd = accept(this->_sockfd, (sockaddr *)&clientinfo, &csize);
	tmp.events = POLLIN;
	std::cout << "New user connected from port : " << clientinfo.sin_port << std::endl;
	this->_vecPoll.push_back(tmp);
	client.setFd(tmp.fd);
	this->_allClients.push_back(client);
	return (true);
}

void	Server::Processmessage (int i)
{
	char				buff[MSG_BUFFERSIZE + 1];
	int					retval;
	iRCMessage			parsedMess;

	memset (buff, 0, MSG_BUFFERSIZE);
	retval = recv(this->_vecPoll[i].fd, buff, MSG_BUFFERSIZE, 0);
	if (retval == -1)
	{
		std::cout << "Recv error" << std::endl;
	}
	// std::cout << buff << std::endl;
	parsedMess = parseMessage(buff);
	if (!this->getAllClients()[i].isRegistered()
		&& parsedMess.command != PASS && parsedMess.command != NICK && parsedMess.command != USER)
	{
		send(this->getAllClients()[i].getFd(), ":server 451 :user not registered yet\r\n", 39, 0);
		return;
	}
	dispatchCommand(parsedMess, this->getAllClients()[i], *this);
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

