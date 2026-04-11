/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:41:51 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/11 19:22:42 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "parser.hpp"

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

std::vector<Channel>& Server::getAllChans()
{
	return (this->_allChannels);
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
		std::cout << "Binding failed" << std::endl;
	if (listen(this->_sockfd, 10) == 0) // socket is in listen mode 10 en file d'attente
		std::cout << "Listen successful"<<  std::endl;
	else
		std::cout << "Listen failed" << std::endl;
}

bool	Server::NewUserHandling(sockaddr_in& clientinfo, socklen_t&  csize)
{
	struct pollfd	tmp;
	Client			client;

	tmp.fd = accept(this->_sockfd, (sockaddr *)&clientinfo, &csize);
	tmp.events = POLLIN;
	tmp.revents = 0;
	std::cout << "New user connected from port : " << clientinfo.sin_port << std::endl;
	this->_vecPoll.push_back(tmp);
	client.setFd(tmp.fd);
	this->_allClients.push_back(client);
	return (true);
}

Client &Server::findClient(int fd, bool	&success)
{
	int		fakeIdx = this->_allClients.size() - 1;

	success = false;
	for (size_t idx = 0; idx < this->_allClients.size(); ++idx)
	{
		if (this->_allClients[idx].getFd() == fd)
		{
			success = true;
			return (this->_allClients[idx]);
		}
		fakeIdx = idx;
	}
	return (this->_allClients[fakeIdx]);
}

Client &Server::findClient(std::string nick, bool	&success)
{
	int		fakeIdx = this->_allClients.size() - 1;

	success = false;
	for (size_t idx = 0; idx < this->_allClients.size(); ++idx)
	{
		if (this->_allClients[idx].getNick() == nick)
		{
			success = true;
			return (this->_allClients[idx]);
			fakeIdx = idx;
		}
	}
	return (this->_allClients[fakeIdx]);
}
Channel &Server::findChan(std::string name, bool &success)
{
	int		chanIndex 	= -1;
	success 	= false;

	if (this->_allChannels.empty())
		return (this->_allChannels[chanIndex]);
	for (size_t idx = 0; idx < this->_allChannels.size(); ++idx)
	{
		if (this->_allChannels[idx].getName() == name)
		{
			success = true;
			chanIndex = static_cast<int>(idx);
			return (this->_allChannels[chanIndex]);
		}
	}
	return (this->_allChannels[chanIndex]);
}

bool		HasCRLF(std::string	str)
{
	std::string		tmp;
	size_t			ret;

	ret = str.find("\r\n");
	if (ret != std::string::npos)
	{
		return (1);
	}
	return (0);
}

//std::string	getCommandCRLF(std::string str)
//{
//	size_t pos;
//	std::string result;

//	pos = str.find(CRLF);
//	result = str.substr(0, pos);

//	std::cout << RED << result << RESET << std::endl;
//	return (result);
//}

int countOccurrences(const std::string& text, const std::string& target) {
    int count = 0;
    size_t pos = text.find(target, 0); // Start searching from index 0
    while (pos != std::string::npos) {
        count++;
        pos = text.find(target, pos + target.length()); // Move past current match
    }
    return count;
}

void	Server::Processmessage (int i)
{
	char						buff[MSG_BUFFERSIZE + 1];
	int							retval;
	iRCMessage					parsedMess;
	std::vector<std::string> 	allMess;
	static	std::string			stock;

	memset (buff, 0, MSG_BUFFERSIZE + 1);
	retval = recv(this->_vecPoll[i].fd, buff, MSG_BUFFERSIZE, 0);
	if (retval == -1)
	{
		std::cout << "Recv error" << std::endl;
		/*disconnect*/
	}
	else if (retval == 0)
	{
		std::cout << "Disconnected client" << std::endl;
		/*disconnnect*/
	}
	
	bool a;
	Client& cl = this->findClient(this->_vecPoll[i].fd, a);

	//cl.setReadBuffer(buff);
	if (!HasCRLF(buff))
	{
		cl.ConcatenateRBuffer(buff);
		return ;
	}
	std::string recvBuf;

	cl.ConcatenateRBuffer(buff);
	recvBuf = cl.getReadBuffer();
	size_t		count;
	count = countOccurrences(recvBuf, CRLF);
	std::vector<std::string> messages = splitCRLF(recvBuf);
	bool foundClnt;
	for (size_t m = 0; m < count; ++m)
	{
		Client& c = this->findClient(this->_vecPoll[i].fd, foundClnt);
		if (!foundClnt)
		{
			std::cout << "no such client" << std::endl;
			return ;
		}
		parsedMess = parseMessage(messages[m]);
		dispatchCommand(parsedMess, c, *this);
	}
	std::cout << BLUE << "Processed = " << recvBuf << RESET << std::endl;
	size_t		pos;
	pos = cl.getReadBuffer().rfind("\r\n");
	if (pos != std::string::npos)
	{
		std::cout << "UPDATE RBUFF = " << &recvBuf[pos + 2] << std::endl;
		cl.setReadBuffer(&recvBuf[pos + 2]);
	}
	this->_vecPoll[i].events |= POLLOUT;
}

void	Server::broadcast(std::string &mess, const Client &caster, const Channel &chan)
{
	std::set<std::string> members = chan.getMembers();
	for (std::set<std::string>::iterator it = members.begin(); it != members.end(); ++it)
	{
		bool found = false;
		Client &cl = this->findClient(*it, found);
		if (found && cl.getFd() != caster.getFd())
		{
			cl.ConcatenateWBuffer(mess);
		}
	}
}
