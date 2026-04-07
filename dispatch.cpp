/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 09:57:17 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/07 10:25:18 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatch.hpp"

void	dispatchCommand(const iRCMessage &mess)
{
	//1-if !registered && !registering_command:return;
	if (!isMessValid(mess))
		return ;
	switch (mess.command)
	{
		case (KICK):
			//KICK command handler;
			break;
		case (TOPIC):
			//TOPIC command handler
			break;
		case (MODE):
			//MODE command handler
			break;
		case (JOIN):
			//JOIN command handler
			break;
		case (PART):
			//PART command handler
			break;
		case (PRIVMSG):
			//PRIVMSG command handler
			break;
		case (QUIT):
			//QUIT command handler
			break;
		default: break;
	}
}
