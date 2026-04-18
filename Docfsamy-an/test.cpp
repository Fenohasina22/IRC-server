/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/18 16:58:57 by mratsima         ###   ########.fr       */
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
	std::cout << "password: " << argv[1] <<  std::endl;
	std::cout << "port: " 	  << argv[2] <<  std::endl;


	/*socket() cree un telephone;
		bind assigne un numero au telephone
	*/

	int fd;
	/*

	We use struct sockaddr_in to initialize what we need to bind
	We don't directly use sockaddr cuz it's harder to initialize and not optimal
	So we use sockaddr_in then cast it to sockaddr ,

	*/
	struct sockaddr_in addr;


	addr.sin_family = AF_INET; // Address Family - Internet Protocol v4 (IPv4)
	addr.sin_port = htons(6667); // conversion
	addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0, There are others macro to change this but we initilize it like that to begin

	fd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM is for TCP
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
	{
		std::cout << "Binding successfull" << std::endl;
	}
	else
	{
		std::cout << "binding failed" << std::endl;
	}
	std::cout << "fd  = " << fd << std::endl;
	// sleep(1000000);
	if (listen(fd, 10) == 0) // 10 personnes en file d'attente, listen cre une file d'attente d'une taille 10
	{
		std::cout << "Listen successful"<<  std::endl;
	}
	else
	{
		std::cout << "Listen failed" << std::endl;
	}
	/*
		accept initialize a sockaddr_in struct and returns an fd of a client
		We can add NULL to begin with

		By default accept waits for client to interact
	*/
	int clientfd;
	struct sockaddr_in clientinfo;
	socklen_t	clientinfosize;

	clientinfosize = sizeof(struct sockaddr_in);

	clientfd = accept(fd, (struct sockaddr*)&clientinfo, &clientinfosize);
	if (clientfd != -1)
	{
		std::cout << "Client port: " << clientinfo.sin_port << std::endl; // exmaple
	}
	else
	{
		std::cout << "Hey there" << std::endl;
	}


	struct pollfd tabfd[10]; // peut contenir 1 socket listen et 9 socket clients crees via accept

	/* 0 is  always for the listening one*/

	tabfd[0].fd = fd;
	tabfd[0].events = POLLIN; // ready to read
	/*revent will be filled by poll*/

	int ret;

	ret = poll(tabfd, 10, -1); // -1 is infinite

	if (ret ==  -1)
	{
		std::cout << "error: in poll" << std::endl;
		return (0);
	}
	else if (ret == 0)
	{
		std::cout << "Timeout" << std::endl;
	}



	/*
	main loop for poll
	*/


	return (0);
}
