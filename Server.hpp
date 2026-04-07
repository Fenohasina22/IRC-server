/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:15 by fsamy-an          #+#    #+#             */
/*   Updated: 2026/04/07 16:36:07 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <poll.h> // pour poll 
#include <sys/socket.h> // pour socket , bind , etc
#include <netinet/in.h>
#include <unistd.h>
#include <vector>


void	InitializeServer(int& sockfd , sockaddr_in& addr);
void	NewUserHandling(int sockfd, sockaddr_in& clientinfo, socklen_t&  csize,std::vector<pollfd>& vecpol);


#endif

