# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alkozma <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/13 18:06:36 by alkozma           #+#    #+#              #
#    Updated: 2020/03/03 05:07:40 by alkozma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ../../ft_otool

SRCS = otool.c \
	   main.c \
	   otool32.c \
	   ../nm/file.c \
	   ../nm/file32.c \
	   ../nm/util.c

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
