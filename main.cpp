/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 16:27:58 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "dispatch.hpp"
#include "parser.hpp"

std::vector<int>	AllFds;
bool				signalCaught = false;

bool	getParams(char **argv, Server &serv)
{
	char		*endptr;
	int			newPort;
	std::string newPass;
	errno = 0;
	newPass = argv[2];
	newPort = std::strtol(argv[1], &endptr, 10);
	if (errno)
		return (false);
	if (*endptr != 0 || endptr == argv[1])
		return (false);
	if (newPort < 1024 || newPort > 65535)
		return (false);
	serv.setPort(newPort);
	serv.setPass(newPass);
	return (true);
}

void	signalHandler(int sig)
{
	(void)sig;
	for (unsigned int i = 0; i < AllFds.size(); i++)
	{
		close (AllFds[i]);
	}
	signalCaught = true;
}

void	SendtoCorrectClient(int i, Server& serv)
{
	bool success;
	Client *c = NULL;

	c = &(serv.findTrueClient(serv.getVecPoll()[i].fd, success));
	if (!success)
		c = &(serv.findClient(serv.getVecPoll()[i].fd, success));
	if (success && c != NULL)
	{
		std::cout << GREEN << "fd = " << serv.getVecPoll()[i].fd << RESET << std::endl;
		std::cout << "Message sent to client |" << (*c).getWriteBuffer() << "|" << std::endl;
		send(serv.getVecPoll()[i].fd, (*c).getWriteBuffer().c_str(), (*c).getWriteBuffer().size(), 0);
		(*c).setWriteBuffer("");
		serv.getVecPoll()[i].events &= ~POLLOUT;
	}
}

void	WatchEvents(sockaddr_in& clientinfo, Server& server)
{
	std::vector<pollfd>& vecpol = server.getVecPoll();
	socklen_t	c_size;

	c_size = sizeof(sockaddr_in);
	for (unsigned int i = 0; i < vecpol.size(); i++)
	{
		if (vecpol[i].fd == server.getSockfd() && (vecpol[i].revents & POLLIN))
		{
			server.NewUserHandling(clientinfo, c_size);
		}
		else if (vecpol[i].revents & POLLIN)
		{
			server.Processmessage(i);
		}
		if (vecpol[i].revents & POLLOUT)
		{
			SendtoCorrectClient(i, server);
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ft_irc <port> <pass>" << std::endl;
		return (1);
	}
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signalHandler);

	Server				server;
	sockaddr_in			clientinfo;
	pollfd				sock; // for initialization of the first element of vecpol

	if (!getParams(argv, server)) // remove errno
		return (1);
		/*print smth nice*/
	if (server.Initialize())
		return (0);
	sock.events = POLLIN;
	sock.fd =  server.getSockfd();
	sock.revents = 0;
	// ensure revents is initialized to avoid valgrind uninitialized use
	std::vector<pollfd>&	vecpol = server.getVecPoll();
	vecpol.push_back(sock);
	while (1)
	{
		// poll SLEEPS AND WAKE UP WHEN AN EVENT HAPPENS LIKE POLLIN
		if (signalCaught || poll(&vecpol[0], vecpol.size(), -1) < 0)
			return (1);
		WatchEvents(clientinfo, server);
	}
	return (0);
}
