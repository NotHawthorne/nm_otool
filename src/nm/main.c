/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:28:07 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/09 04:07:24 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"

int		main(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc == 2)
		ft_nm(argv[i]);
	else if (argc >= 2)
	{
		ft_printf("\n");
		while (i < argc)
		{
			ft_printf("%s:\n", argv[i]);
			ft_nm(argv[i++]);
			if (i < argc)
				ft_printf("\n");
		}
	}
	else
		ft_printf("usage: nm [file]\n");
}

int		ft_nm(char *file)
{
	t_nm					info;
	t_nm32					info32;
	void					*mem;

	ft_nm_init(&info, &info32);
	if (!(mem = nmo_load(file, &(info.stat), 0)))
		return (0);
	info.header = mem;
	if (info.header->magic != MH_MAGIC_64 && info.header->magic != MH_CIGAM_64
		&& info.header->magic != MH_MAGIC && info.header->magic != MH_CIGAM)
		if (!(mem = nmo_find_header(mem)))
			return (ft_printf("nm: could not find header\n"));
	info32.header = mem;
	info.header = mem;
	if (info32.header->magic == MH_MAGIC || info32.header->magic == MH_CIGAM)
		return (ft_nm32(&info32));
	if (info.header->magic == MH_MAGIC_64 || info.header->magic == MH_CIGAM_64)
	{
		info.sections = nm_dump((void*)(info.header + 1), &info, 0);
		nm_symbols(info.symtab_start, info.header, &info);
		munmap(info.header, info.stat.st_size);
		nm_print(info.symbols, 0);
		return (1);
	}
	return (ft_printf("nm: invalid file: %s\n", file));
}

int		ft_nm32(t_nm32 *info)
{
	int						os;

	if (info->header->magic == MH_MAGIC || info->header->magic == MH_CIGAM)
	{
		info->sections = nm_dump32((void*)(info->header + 1), info, 0, &os);
		nm_symbols32(info->symtab_start, info->header, info);
		munmap(info->header, info->stat.st_size);
		nm_print(info->symbols, 1);
		return (1);
	}
	return (0);
}
