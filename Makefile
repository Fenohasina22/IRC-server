# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fsamy-an <fsamy-an@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/07 21:06:09 by fsamy-an          #+#    #+#              #
#    Updated: 2026/04/07 21:36:24 by fsamy-an         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = ft_irc

CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = Channel.cpp 				\
		Client.cpp				\
		main.cpp				\
		Server.cpp				

SRCC =	commands.cpp 	\
		dispatch.cpp	\
		parser.cpp		


OBJ = $(SRCS:%.cpp=%.o)
OBJC = $(SRCC:%.cpp=%.o)

all : $(NAME)

$(NAME): $(OBJ)  $(OBJC)
	$(CPP) $(CPPFLAGS) $(OBJ) $(OBJC) -o $(NAME)

$(OBJ) : $(SRCS)
	$(CPP) $(CPPFLAGS) -c $(SRCS)

$(OBJC) : $(SRCC)
	$(CPP) $(CPPFLAGS) -c mratsima/$(SRCC)

clean : 
	rm -rf $(OBJ) $(OBJC)

fclean : clean 
	rm -rf $(NAME)

re : fclean all