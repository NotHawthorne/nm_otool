# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alkozma <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/13 18:06:36 by alkozma           #+#    #+#              #
#    Updated: 2020/03/03 05:02:55 by alkozma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ../../ft_nm

SRCS = file.c \
	   main.c \
	   file32.c \
	   util.c

OBJECTS = $(patsubst %.c,%.o,$(SRCS))

INCLUDES = ../../includes

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	gcc $(SRCS) -I$(INCLUDES) -L../../ -lft $(FLAGS) -o $(NAME)

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
