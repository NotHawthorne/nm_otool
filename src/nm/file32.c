/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:28:47 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/03 05:09:37 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"
#include <mach-o/nlist.h>
#include <string.h>

char	nm_symbolize32(struct nlist *l, t_nm32 *info)
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

void	nm_insert_symbol32(t_nm32 *info, t_nm_symbol *cur, t_nm_symbol *s)
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
				nm_insert_symbol32(info, cur->r, s);
		}
		else
		{
			if (!cur->l)
				cur->l = s;
			else
				nm_insert_symbol32(info, cur->l, s);
		}
	}
}

int		nm_symbols32(void *start, struct mach_header *h, t_nm32 *info)
{
	struct symtab_command	*c;
	struct nlist			*l;
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
		s->type = nm_symbolize32(l, info);
		s->undef = ((l->n_type & N_TYPE) == N_UNDF && !l->n_value);
		nm_insert_symbol32(info, info->symbols, s);
		l = (void*)l + sizeof(struct nlist);
	}
	return (1);
}

void	nm_section_it32(void *start, int nsects, t_nm32 *info)
{
	struct section	*s;

	s = start;
	while (nsects)
	{
		memcpy(info->stab[info->sections++], s->sectname, 16);
		s = ((void*)s + sizeof(struct section));
		nsects--;
	}
}

int		nm_dump32(void *start, t_nm32 *info, int depth, int *os)
{
	struct segment_command		*c;
	struct load_command			*lc;

	lc = start;
	if (lc->cmd == LC_SYMTAB)
		info->symtab_start = start;
	c = start;
	if (lc->cmd == LC_SEGMENT)
		if (c->nsects)
			nm_section_it32((void*)(c + 1), c->nsects, info);
	if ((uint32_t)depth < info->header->ncmds)
	{
		*os += c->cmdsize;
		return (c->nsects + nm_dump32(start + c->cmdsize, info, depth + 1, os));
	}
	return (0);
}
