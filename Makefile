# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/18 11:50:31 by ebenoist          #+#    #+#              #
#    Updated: 2026/06/20 15:07:38 by ebenoist         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ft_ping

CC		= cc -g
CFLAGS	= -Wall -Werror -Wextra -g
SRC		= src/main.c\
		  src/init.c\
		  src/util.c\

			 
OBJ		= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re