/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:41:51 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/07 16:34:47 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


void	InitializeServer(int& sockfd , sockaddr_in& addr)
{
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
}

void	NewUserHandling(int sockfd, sockaddr_in& clientinfo, socklen_t&  csize,std::vector<pollfd>& vecpol)
{
	struct pollfd tmp;

	tmp.fd = accept(sockfd, (sockaddr *)&clientinfo, &csize);
	tmp.events = POLLIN;
	std::cout << "New user connected from port : " << clientinfo.sin_port << std::endl;
	vecpol.push_back(tmp);
}
