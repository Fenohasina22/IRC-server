/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 16:37:35 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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


	InitializeServer(sockfd, addr);

	struct pollfd sock;
	clientinfosize = sizeof(sockaddr_in);
	sock.fd = sockfd;
	sock.events = POLLIN;
	vecpol.push_back(sock);
	while (1)
	{
		ret = poll(&vecpol[0], vecpol.size(), -1); // SLEEPS AND WAKE UP WHEN AN EVENT HAPPENS LIKE POLLIN
		if (ret < 0)
		{
			std::cout << "error: poll"<< std::endl;
			return (0);
		}
		for (int i = 0; i < vecpol.size(); i++)
		{
			if (vecpol[i].fd == sockfd && (vecpol[i].revents & POLLIN))
			{
				NewUserHandling(sockfd, clientinfo, clientinfosize, vecpol);
			}
			else if (vecpol[i].revents & POLLIN)
			{
				char buff[5];
				int		retval;
				retval = recv(vecpol[i].fd, buff, 5, 0);
				if (retval == -1)
				{
					std::cout << "Recve error" << std::endl;
				}
				std::cout << "Buff = " << buff << std::endl;
			}
		}
	}

	return (0);
}
