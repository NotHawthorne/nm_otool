/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 00:43:59 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/09 04:11:15 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"
#include <string.h>

void	otool_print_data(void *start, struct section_64 *s)
{
	unsigned long	traversed;

	traversed = 0;
	while (traversed < s->size + ((s->size % 16) ? (16 - s->size % 16) : 0))
	{
		if (traversed == 0)
			ft_printf("Contents of (__TEXT,__text) section\n");
		if (traversed % 16 == 0)
			ft_printf("%016llx\t", (s->addr + traversed));
		if (traversed < s->size)
		{
			if (0xff & *((char*)(start + traversed)))
				ft_printf("%02x ", 0xff & *((char*)(start + traversed)));
			else
				ft_printf("00 ");
		}
		if (traversed % 16 == 15)
			ft_printf("\n");
		traversed++;
	}
}

int		otool_section_check(t_nm *info, void *start, int nsects)
{
	struct section_64	*s;

	s = start;
	while (nsects)
	{
		if (!ft_strcmp(s->sectname, "__text"))
		{
			otool_print_data((void*)info->header + s->offset, s);
			return (1);
		}
		s = (void*)((char*)s + sizeof(struct section_64));
		nsects--;
	}
	return (0);
}

int		otool_findtext(t_nm *info)
{
	struct segment_command_64	*c;
	struct load_command			*lc;
	uint32_t					cmds;

	lc = (void*)(info->header + 1);
	cmds = info->header->ncmds;
	while (cmds)
	{
		if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
		{
			c = (void*)lc;
			if (otool_section_check(info, c + 1, c->nsects))
				return (1);
		}
		cmds--;
		lc = ((void*)lc + lc->cmdsize);
	}
	return (0);
}

int		ft_otool(char *file)
{
	void	*mem;
	t_nm	info;
	t_nm32	info32;

	if (!(mem = nmo_load(file, &(info.stat), 1)))
		return (0);
	info.header = mem;
	if (info.header->magic != MH_MAGIC_64 && info.header->magic != MH_CIGAM_64
		&& info.header->magic != MH_MAGIC && info.header->magic != MH_CIGAM)
		if (!(mem = nmo_find_header(mem)))
			return (ft_printf("otool: could not find header: %s\n", file));
	info32.header = mem;
	info.header = mem;
	if (info32.header->magic == MH_MAGIC || info32.header->magic == MH_CIGAM)
		return (ft_otool32(file, &info32));
	if (info.header->magic == MH_MAGIC_64 || info.header->magic == MH_CIGAM_64)
	{
		ft_printf("%s:\n", file);
		otool_findtext(&info);
		munmap(info.header, info.stat.st_size);
	}
	else
		ft_printf("%s: is not an object file\n", file);
	return (1);
}
