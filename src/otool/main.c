/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 02:40:29 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/03 04:36:48 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"

int		main(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc >= 2)
		while (i != argc)
			ft_otool(argv[i++]);
	else
		ft_printf("usage: otool [file]\n");
}
