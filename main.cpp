/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 10:57:00 by mratsima         ###   ########.fr       */
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
	struct pollfd		tabfd[10];
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

	/*INITIALISATION CLIENT INFO
	clientinfosize = sizeof(struct sockaddr_in);
	clientfd = accept(sockfd, (struct sockaddr*)&clientinfo, &clientinfosize); // return fd for a client and initialize clientinfo
	if (clientfd != -1)
		std::cout << "Client port: " << clientinfo.sin_port << std::endl;
	else
		std::cout << "Hey there" << std::endl;*/

	while (1)
	{

		tabfd[0].fd = sockfd;
		tabfd[0].events = POLLIN;
		ret = poll(tabfd, 10, -1);
		if (ret ==  -1)
		{
			std::cout << "error: in poll" << std::endl;
			return (0);
		}
		else if (ret == 0)
		{
			std::cout << "Timeout" << std::endl;
			return (0);
		}

	}
	return (0);
}
