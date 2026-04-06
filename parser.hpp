/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mratsima <mratsima@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:53:40 by mratsima          #+#    #+#             */
/*   Updated: 2026/04/06 09:43:40 by mratsima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <sstream>
# include <vector>
# include <string>

struct message
{
	std::string 				prefix;
	std::string 				command;
	std::vector<std::string>	args;
	std::string 				CRLF;
	size_t						len;
};

#endif
