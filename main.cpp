/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/19 08:11:05 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "dispatch.hpp"
#include "parser.hpp"
#include <climits>

std::vector<int>	AllFds;
bool				signalCaught = false;

void ircIntro() {
    std::cout << BLUE << "\n"
         << "   ███████╗███████╗███████╗███╗   ██╗\n"
         << "   ██╔════╝██╔════╝██╔════╝████╗  ██║\n"
         << "   ███████╗█████╗  █████╗  ██╔██╗ ██║\n"
         << "   ╚════██║██╔══╝  ██╔══╝  ██║╚██╗██║\n"
         << "   ███████║███████╗███████╗██║ ╚████║\n"
         << "   ╚══════╝╚══════╝╚══════╝╚═╝  ╚═══╝\n" << RESET;

    std::cout << BLUE << "\n"
         << "     \n"
         << "	  █▓▒░ seen.nowhere ░▒▓█\n"
         << "  \n"
         << "   " << BLUE << "Internet Relay Chat Server in C/C++\n"
         << "  \n"
         << "   " << GREEN << "Status: ONLINE" << RESET << "\n\n";

    std::cout << BLUE << "╔══════════════════════════════════════╗\n"
         << "║ Server: " << GREEN << "seen.nowhere                 " << BLUE << "║\n"
         << "║ Devs: " << GREEN << "fsamy-an, mratsima             " << BLUE << "║\n"
         << "║ Address family: " << GREEN << "IPv4                 " << BLUE << "║\n"
         << "╚══════════════════════════════════════╝" << RESET << "\n\n";
}

bool	getParams(char **argv, Server &serv)
{
	char    	*endptr = NULL;
	int     	newPort = 0;
	std::string newPass;
	long    	testPort = 0;

	newPass = argv[2];
	const char *s = argv[1];
	testPort = std::strtol(s, &endptr, 10);
	if (endptr == s || *endptr != '\0')
		return (false);
	if (testPort == LONG_MAX || testPort == LONG_MIN)
		return (false);
	if (testPort < 1024 || testPort > 65535)
		return (false);
	newPort = static_cast<int>(testPort);
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
		std::cerr << BOLD << RED << "Usage: ./ft_irc <port> <pass>" << RESET << std::endl;
		return (1);
	}
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signalHandler);
	ircIntro();
	Server				server;
	sockaddr_in			clientinfo;
	pollfd				sock;

	if (!getParams(argv, server))
	{
		std::cerr << RED << "Error: Parameter error" << RESET << std::endl;
		return (1);
	}
	if (server.Initialize())
		return (0);
	sock.events = POLLIN;
	sock.fd =  server.getSockfd();
	sock.revents = 0;
	std::vector<pollfd>&	vecpol = server.getVecPoll();
	vecpol.push_back(sock);
	while (1)
	{
		if (signalCaught || poll(&vecpol[0], vecpol.size(), -1) < 0)
			return (1);
		WatchEvents(clientinfo, server);
	}
	return (0);
}
