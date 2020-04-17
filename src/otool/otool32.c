/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/22 00:43:59 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/09 04:11:24 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"
#include <string.h>

void	otool_print_data32(void *start, struct section *s)
{
	unsigned long	traversed;

	traversed = 0;
	while (traversed < s->size + ((s->size % 16) ? (16 - s->size % 16) : 0))
	{
		if (traversed == 0)
			ft_printf("Contents of (__TEXT,__text) section\n");
		if (traversed % 16 == 0)
			ft_printf("%08lx\t", (s->addr + traversed));
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

int		otool_section_check32(t_nm32 *info, void *start, int nsects)
{
	struct section	*s;

	s = start;
	while (nsects)
	{
		if (!ft_strcmp(s->sectname, "__text"))
		{
			otool_print_data32((void*)info->header + s->offset, s);
			return (1);
		}
		s = (void*)((char*)s + sizeof(struct section_64));
		nsects--;
	}
	return (0);
}

int		otool_findtext32(t_nm32 *info)
{
	struct segment_command		*c;
	struct load_command			*lc;
	uint32_t					cmds;

	lc = (void*)(info->header + 1);
	cmds = info->header->ncmds;
	while (cmds)
	{
		if (lc->cmd == LC_SEGMENT)
		{
			c = (void*)lc;
			if (otool_section_check32(info, c + 1, c->nsects))
				return (1);
		}
		cmds--;
		lc = ((void*)lc + lc->cmdsize);
	}
	return (0);
}

int		ft_otool32(char *file, t_nm32 *h)
{
	ft_printf("%s:\n", file);
	otool_findtext32(h);
	munmap(h->header, h->stat.st_size);
	return (1);
}
