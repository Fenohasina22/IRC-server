/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/11 13:16:47 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "dispatch.hpp"
#include "parser.hpp"

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

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Needs more args" << std::endl;
		return (1);
	}
	struct sockaddr_in	clientinfo;
	struct pollfd		sock; // for initialization of the first element of vecpol
	socklen_t			c_size;
	Server				server;
	Client				c;

	if (!getParams(argv, server))
		return (1);
	c_size = sizeof(sockaddr_in);
	server.Initialize();
	sock.events = POLLIN;
	sock.fd =  server.getSockfd();
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
				server.Processmessage(c, i);
			}
			if (vecpol[i].revents & POLLOUT)
			{
				/* Send message */
				send(vecpol[i].fd, c.getWriteBuffer().c_str(), c.getWriteBuffer().size(), 0);
				/*Disable POLLOUT*/
				//std::cout << RED << "CLOSE POLLOUT" << std::endl;
				vecpol[i].events &= ~POLLOUT;
				//std::cout << "--------------------" << std::endl;
				//std::cout << RED << c.getWriteBuffer() << RESET << std::endl;
				//std::cout << "--------------------" << std::endl;
			}
		}
	}
	return (0);
}
