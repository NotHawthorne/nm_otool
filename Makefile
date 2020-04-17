# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alkozma <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/13 18:06:36 by alkozma           #+#    #+#              #
#    Updated: 2020/03/03 05:14:59 by alkozma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := nmotool

all: $(NAME)

$(NAME):
	$(MAKE) -C libft
	$(MAKE) -C src/nm
	$(MAKE) -C src/otool

clean:
	$(MAKE) clean -C libft
	$(MAKE) clean -C src/nm
	$(MAKE) clean -C src/otool

fclean: clean
	$(MAKE) fclean -C libft
	$(MAKE) fclean -C src/nm
	$(MAKE) fclean -C src/otool

re: fclean all
