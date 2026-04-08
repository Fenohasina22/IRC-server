/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:41:51 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/08 09:27:48 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "mratsima/parser.hpp"


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

void						Server::setPort(int newPort)
{
	this->_port = newPort;
}

int						Server::getPort() const
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
	this->_addr.sin_port = htons(this->_port);
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

bool	Server::NewUserHandling(sockaddr_in& clientinfo, socklen_t&  csize)
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

void	printiRCMESS(iRCMessage mess)
{
	(void)mess;
	// std::cout << "prefix = " << mess.prefix << std::endl;
	// std::cout << "command = " << mess.cmd << std::endl;
	// std::cout << "args = ";
	// for (size_t i = 0; i < mess.args.size(); i++)
	// 	std::cout << "-" << mess.args[i] << std::endl;
}

void	Server::Processmessage (int i)
{
	char						buff[MSG_BUFFERSIZE + 1];
	int							retval;
	iRCMessage					parsedMess;
	std::vector<std::string> 	allMess;

	memset (buff, 0, MSG_BUFFERSIZE);
	retval = recv(this->_vecPoll[i].fd, buff, MSG_BUFFERSIZE, 0);
	if (retval == -1)
	{
		std::cout << "Recv error" << std::endl;
	}
	std::cout << buff << std::endl;
	int clientIndex = -1;
	for (size_t idx = 0; idx < this->_allClients.size(); ++idx)
	{
		if (this->_allClients[idx].getFd() == this->_vecPoll[i].fd)
		{
			clientIndex = static_cast<int>(idx);
			break;
		}
	}
	if (clientIndex == -1)
	{
		std::cout << "Unknown client for fd " << this->_vecPoll[i].fd << std::endl;
		return;
	}
	// if (!this->getAllClients()[clientIndex].isRegistered()
	// 	&& parsedMess.command != PASS && parsedMess.command != NICK && parsedMess.command != USER)
	// {
	// 	send(this->getAllClients()[clientIndex].getFd(), ":server 451 :user not registered yet\r\n", 39, 0);
	// 	return;
	// }
	std::string recvBuf(buff, retval);
	std::vector<std::string> messages = splitCRLF(recvBuf);
	for (size_t m = 0; m < messages.size(); ++m)
	{
		parsedMess = parseMessage(messages[m]);
		// if (!this->getAllClients()[clientIndex].isRegistered()
		// 	&& parsedMess.cmd != PASS && parsedMess.cmd != NICK && parsedMess.cmd != USER)
		// {
		// 	send(this->getAllClients()[clientIndex].getFd(), ":server 451 :user not registered yet\r\n", 39, 0);
		// 	continue;
		// }
		printiRCMESS(parsedMess);
		dispatchCommand(parsedMess, this->getAllClients()[clientIndex], *this);
	}
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

