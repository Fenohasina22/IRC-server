/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/13 16:00:31 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "dispatch.hpp"
#include "parser.hpp"

/*Handle ghost user reconnection */

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
	serv.setPort(newPort);
	serv.setPass(newPass);
	return (true);
}

void	signalHandler(int sig)
{
	(void)sig;
	std::cout << "SIGINT catched" << std::endl;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ft_irc <port> <pass>" << std::endl;
		return (1);
	}
	// signal handling 
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signalHandler);
	struct sockaddr_in	clientinfo;
	struct pollfd		sock; // for initialization of the first element of vecpol
	socklen_t			c_size;
	Server				server;

	if (!getParams(argv, server))
		return (1);
	c_size = sizeof(sockaddr_in);
	std::cout << BLUE << std::endl;
	std::cout << " == CREATING SERVER === " << std::endl;
	std::cout << RESET << std::endl;
	if (server.Initialize())
	{
		return (0);
	}
	
	sock.events = POLLIN;
	sock.fd =  server.getSockfd();
	// ensure revents is initialized to avoid valgrind uninitialized use
	sock.revents = 0;
	std::vector<pollfd>&	vecpol = server.getVecPoll();
	vecpol.push_back(sock);
	std::string			message;
	while (1)
	{
		// poll SLEEPS AND WAKE UP WHEN AN EVENT HAPPENS LIKE POLLIN
		if (poll(&vecpol[0], vecpol.size(), -1) < 0)
		{
			std::cout << "error: poll"<< std::endl;
			return (0);
		}
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
				bool success;
				Client &c = server.findClient(vecpol[i].fd, success);
				if (success)
				{
					send(vecpol[i].fd, c.getWriteBuffer().c_str(), c.getWriteBuffer().size(), 0);
					c.setWriteBuffer("");
					vecpol[i].events &= ~POLLOUT;
				}
			}
		}
	}
	return (0);
}
