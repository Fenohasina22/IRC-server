/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 15:20:17 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Needs more args" << std::endl;
		return (1);
	}

	/*SOCKET*/
	struct sockaddr_in	addr;
	int					sockfd;
	/*CLIENT*/
	int					clientfd;
	struct sockaddr_in	clientinfo;
	socklen_t			clientinfosize;
	/*POLL*/
	std::vector<pollfd>	vecpol;
	int					ret;



	/*INITIALISATION SOCKET*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6667);
	addr.sin_addr.s_addr = INADDR_ANY; 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // Creates a socket IPv4, TCP
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0) // adding info to the socket
		std::cout << "Binding successfull" << std::endl;
	else
		std::cout << "binding failed" << std::endl;	
	if (listen(sockfd, 10) == 0) // socket is in listen mode
		std::cout << "Listen successful"<<  std::endl;
	else
		std::cout << "Listen failed" << std::endl;
		

	clientinfosize = sizeof(sockaddr_in);

	int j = 1;
	int i = 0;
	int nfds = 2;
	/*
	Pour bien utiliser poll il faut nettoyer apres reception de siggnal nc via accept si c'est le socket, recv si c'est un client
	
	tabfd[0] = socket;
	*/
	struct pollfd sock;

	sock.fd = sockfd;
	sock.events = POLLIN;

	vecpol.push_back(sock);
	while (1)
	{
		ret = poll(&vecpol[0], vecpol.size(), -1); // SLEEPS AND WAKE UP WHEN AN EVENT HAPPENS LIKE POLLIN
		if (ret < 0)
		{
			std::cout << "error: poll"<< std::endl;
		}
		for (int i = 0; i < nfds; i++)
		{
			if (vecpol[i].fd == sockfd && (vecpol[i].revents & POLLIN))
			{
				struct pollfd tmp;

				tmp.fd = accept(sockfd, (sockaddr *)&clientinfo, &clientinfosize);
				tmp.events = POLLIN;
				std::cout << "New user connected from port : " << clientinfo.sin_port << std::endl;
				vecpol.push_back(tmp);
			}
			else if (vecpol[i].revents & POLLIN)
			{
				/*recv*/
				char buff[5]; // to test

				recv(vecpol[i].fd, buff, 5, 0);
				std::cout << "Buff = " << buff << std::endl;
			}
		}
	}

	return (0);
}
