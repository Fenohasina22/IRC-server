/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/06 14:28:59 by fsamy-an         ###   ########.fr       */
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


	clientfd = accept(fd, NULL, NULL);
	if (clientfd != -1)
	{
		std::cout << "Yayy a client is conected" << std::endl;
	}
	else{
		std::cout << "Hey there" << std::endl;
	}
	// sleep (1000000);
	return (0);
}
