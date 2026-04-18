/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:41:51 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/18 09:08:17 by fsamy-an         ###   ########.fr       */
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

std::vector<Client>&	Server::getTrueClients()
{
	return (this->_trueClients);
}


int	Server::Initialize()
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
		std::cout << RED << "Setsockopt error" << RESET << std::endl;
		return (1);
	}
	if (bind(this->_sockfd, (struct sockaddr *)&(this->_addr), sizeof(this->_addr)) == 0)
		std::cout << GREEN << "Binding successfull" << RESET<< std::endl;
	else
	{
		std::cout << RED <<"Binding failed" << RESET << std::endl;
		close(this->_sockfd);
		return (1);
	}
	if (listen(this->_sockfd, SOMAXCONN) == 0)
		std::cout << GREEN << "Listen successful" << RESET <<  std::endl;
	else
	{
		std::cout << RED << "Listen failed" << RESET << std::endl;
		close (this->_sockfd);
		return (1);
	}
	AllFds.push_back(this->_sockfd);
	return (0);
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
	client.setClientInfos(clientinfo);
	this->_allClients.push_back(client);
	AllFds.push_back(tmp.fd);
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

Client &Server::findTrueClient(int fd, bool	&success)
{
	int		fakeIdx = this->_trueClients.size() - 1;

	success = false;
	for (size_t idx = 0; idx < this->_trueClients.size(); ++idx)
	{
		//std::cout << GREEN << this->_trueClients[idx].getFd() << " == " << fd << RESET << std::endl;
		if (this->_trueClients[idx].getFd() == fd)
		{
			//std::cout << GREEN << "YES" << RESET << std::endl;
			success = true;
			return (this->_trueClients[idx]);
		}
		//std::cout << RED << "NO" << std::endl;
		fakeIdx = idx;
	}
	return (this->_trueClients[fakeIdx]);
}

Client &Server::findTrueClient(std::string nick, bool	&success)
{
	int		fakeIdx = this->_trueClients.size() - 1;

	success = false;
	for (size_t idx = 0; idx < this->_trueClients.size(); ++idx)
	{
		if (this->_trueClients[idx].getNick() == nick)
		{
			success = true;
			return (this->_trueClients[idx]);
			fakeIdx = idx;
		}
	}
	return (this->_trueClients[fakeIdx]);
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

int countOccurrences(const std::string& text, const std::string& target) {
    int count = 0;
    size_t pos = text.find(target, 0); // Start searching from index 0
    while (pos != std::string::npos) {
        count++;
        pos = text.find(target, pos + target.length()); // Move past current match
    }
    return count;
}


int		ParseAndExecute(int i, char *buff, Client& cl, Server& server)
{

	std::vector<std::string>	messages;
	iRCMessage					parsedMess;
	std::string					recvBuf;
	size_t						count;
	bool						foundClnt;
	Client						*c;
	bool						validPass;

	c = NULL;
	if (!cl.getPassState())
		validPass = false;
	else
		validPass = true;
	cl.ConcatenateRBuffer(buff);
	recvBuf = cl.getReadBuffer();
	count = countOccurrences(recvBuf, CRLF);
	messages = splitCRLF(recvBuf);
	for (size_t m = 0; m < count; ++m)
	{
		if (!validPass)
			c = &(server.findClient(server.getVecPoll()[i].fd, foundClnt));

		else
			c = &(server.findTrueClient(server.getVecPoll()[i].fd, foundClnt));
		if (!foundClnt)
		{
			std::cout << "no such client" << std::endl;
			return (1);
		}
		parsedMess = parseMessage(messages[m]);
		dispatchCommand(parsedMess, *c, server, validPass);
	}
	return (0);
}


void	Server::Processmessage (int i)
{
	char			buff[MSG_BUFFERSIZE + 1];
	int				retval;
	bool			success;


	memset (buff, 0, MSG_BUFFERSIZE + 1);
	//std::cout << YELLOW << buff << RESET << std::endl;
	
	retval = recv(this->_vecPoll[i].fd, buff, MSG_BUFFERSIZE, 0);
	if (retval == -1 || retval == 0)
	{
		std::cout << GREEN << "Client disconnected" << RESET << std::endl;
		CleanUp(*this, i);
		return ;
	}
	Client& cl = this->findClient(this->_vecPoll[i].fd, success);
	if (!HasCRLF(buff))
	{
		cl.ConcatenateRBuffer(buff);
		return ;
	}
	if (ParseAndExecute(i, buff, cl, *this))
		return ;

	size_t		pos;
	pos = cl.getReadBuffer().rfind("\r\n");
	if (pos != std::string::npos)
	{
		cl.setReadBuffer(&cl.getReadBuffer()[pos + 2]);
	}
}

void	Server::deleteChan(std::string &chanName)
{
	for (std::vector<Channel>::iterator it = this->_allChannels.begin();
		it != this->_allChannels.end(); )
	{
		if (it->getName() == chanName)
		{
			std::set<std::string> members = it->getMembers();
			for (std::set<std::string>::iterator mit = members.begin(); mit != members.end(); ++mit)
			{
				bool found = false;
				Client &cl = this->findTrueClient(*mit, found);
				if (found)
					cl.removeChannel(chanName);
			}
			it = this->_allChannels.erase(it);
		}
		else
			++it;
	}
}

void	Server::broadcast(std::string &mess, const Client &caster, const Channel &chan, Server& serv)
{
	std::set<std::string> members = chan.getMembers();
	for (std::set<std::string>::iterator it = members.begin(); it != members.end(); ++it)
	{
		bool found = false;
		Client &cl = this->findTrueClient(*it, found);
		if (found && cl.getFd() != caster.getFd())
		{
			cl.ConcatenateWBuffer(mess, serv);
		}
	}
}

void	Server::broadcastWithoutChan(std::string &mess, const Client &caster, std::set<std::string> members, Server& serv)
{
	for (std::set<std::string>::iterator it = members.begin(); it != members.end(); ++it)
	{
		bool found = false;
		Client &cl = this->findTrueClient(*it, found);
		if (found && cl.getFd() != caster.getFd())
		{
			//std::cout << YELLOW <<"sent to" << cl.getNick() << RESET<< std::endl;
			cl.ConcatenateWBuffer(mess, serv);
		}
	}
}

pollfd&	Server::findElementByfd(int fd, bool& a)
{
	std::vector<pollfd>&	vec = getVecPoll();
	unsigned int i;

	for (i = 1; i < vec.size(); i++)
	{
		if (vec[i].fd == fd)
		{
			a = true;
			return (vec[i]);
		}
	}
	a = false;
	return (vec[i]);
}
