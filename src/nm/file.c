/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:28:47 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/09 04:01:46 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <string.h>

void		*nmo_find_header(void *start)
{
	struct fat_header		*h;
	struct fat_arch			*a;
	struct mach_header_64	*ret;
	int						i;
	struct mach_header_64	*altret;

	h = start;
	i = 0;
	altret = 0;
	if (h->magic != FAT_MAGIC && h->magic != FAT_CIGAM)
		return (NULL);
	a = (void*)(h + 1);
	while (i++ != switch_endian32(h->nfat_arch))
	{
		ret = (start + switch_endian32(a->offset));
		if (ret->magic == MH_MAGIC_64 || ret->magic == MH_CIGAM_64)
			return ((void*)ret);
		else if (ret->magic == MH_MAGIC || ret->magic == MH_CIGAM)
			altret = ((void*)ret);
		a++;
	}
	return (altret);
}

char		nm_symbolize(struct nlist_64 *l, t_nm *info)
{
	int	tmp;
	int	desc;
	int	ext;

	tmp = l->n_type & N_TYPE;
	ext = l->n_type & N_EXT;
	desc = l->n_desc & ~REFERENCE_TYPE;
	if (tmp == N_UNDF)
	{
		if ((l->n_type & N_EXT) != 0 && l->n_value != 0)
			return ('C');
		return ('U');
	}
	else if (tmp == N_ABS)
		return (desc == REFERENCE_FLAG_DEFINED ? 'A' : 'a');
	else if (tmp == N_SECT)
	{
		if (!ft_strcmp(info->stab[l->n_sect - 1], "__text"))
			return (ext ? 'T' : 't');
		else if (!ft_strcmp(info->stab[l->n_sect - 1], "__data"))
			return (ext ? 'D' : 'd');
		else if (!ft_strcmp(info->stab[l->n_sect - 1], "__bss"))
			return (ext ? 'B' : 'b');
	}
	return (ext ? 'S' : 's');
}

void		nm_insert_symbol(t_nm *info, t_nm_symbol *cur, t_nm_symbol *s)
{
	int	res;

	if (!cur)
		info->symbols = s;
	else
	{
		res = ft_strcmp(cur->name, s->name);
		if (res < 0)
		{
			if (!cur->r)
				cur->r = s;
			else
				nm_insert_symbol(info, cur->r, s);
		}
		else
		{
			if (!cur->l)
				cur->l = s;
			else
				nm_insert_symbol(info, cur->l, s);
		}
	}
}

int			nm_symbols(void *start, struct mach_header_64 *h, t_nm *info)
{
	struct symtab_command	*c;
	struct nlist_64			*l;
	t_nm_symbol				*s;
	uint32_t				i;

	c = start;
	l = (void*)h + c->symoff;
	i = 0;
	while (i++ != c->nsyms)
	{
		s = malloc(sizeof(*s));
		s->name = ft_strdup((void*)h + c->stroff + l->n_un.n_strx);
		s->l = 0;
		s->r = 0;
		s->val = l->n_value;
		s->type = nm_symbolize(l, info);
		s->undef = ((l->n_type & N_TYPE) == N_UNDF && !l->n_value);
		nm_insert_symbol(info, info->symbols, s);
		l = (void*)l + sizeof(struct nlist_64);
	}
	return (1);
}

int			nm_dump(void *start, t_nm *info, int depth)
{
	struct segment_command_64	*c;
	struct load_command			*lc;

	lc = start;
	if (lc->cmd == LC_SYMTAB)
		info->symtab_start = start;
	c = start;
	if (lc->cmd == LC_SEGMENT_64 || lc->cmd == LC_SEGMENT)
		if (c->nsects)
			nm_section_it((void*)(c + 1), c->nsects, info);
	if ((uint32_t)depth < info->header->ncmds)
		return (c->nsects + nm_dump(start + c->cmdsize, info, depth + 1));
	return (0);
}
