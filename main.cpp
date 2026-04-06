/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:02:43 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/06 13:17:24 by fsamy-an         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3)
		return (1);
	std::cout << "password: " << argv[1] <<  std::endl;
	std::cout << "port: " 	  << argv[2] <<  std::endl;


	/*
		I need to create a main loop using poll()
	
	*/


	return (0);
}
