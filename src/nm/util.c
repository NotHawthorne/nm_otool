/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:28:47 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/09 04:09:20 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/nmotool.h"
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <string.h>

int32_t		switch_endian32(int32_t num)
{
	int	ret;

	ret = ((num >> 24) & 0xff) |
	((num << 8) & 0xff0000) |
	((num >> 8) & 0xff00) |
	((num << 24) & 0xff000000);
	return (ret);
}

void		*nmo_load(char *file, struct stat *s, int prog)
{
	int				fd;
	void			*ret;
	int				code;
	static char		nms[] = "nm";
	static char		ots[] = "otool";

	code = 0;
	if ((fd = open(file, O_RDONLY)) < 0)
		code = ft_printf("%s: open failed: %s\n", prog ? ots : nms, file);
	else if ((fstat(fd, s)) < 0)
		code = ft_printf("%s: stat failed: %s\n", prog ? ots : nms, file);
	else if ((ret = mmap(0, s->st_size, PROT_READ, MAP_PRIVATE,
					fd, 0)) == MAP_FAILED)
		code = ft_printf("%s: mmap failed\n", prog ? ots : nms);
	close(fd);
	return (code ? NULL : ret);
}

void		nm_print(t_nm_symbol *cur, int is32)
{
	if (cur->l)
		nm_print(cur->l, is32);
	if (cur->undef)
		ft_printf(is32 ? "         " : "                 ");
	else
		ft_printf(is32 ? "%.8llx " : "%.16llx ", cur->val);
	ft_printf("%c %s\n", cur->type, cur->name);
	if (cur->r)
		nm_print(cur->r, is32);
	free(cur->name);
	free(cur);
	cur = NULL;
}

void		nm_section_it(void *start, int nsects, t_nm *info)
{
	struct section_64 *s;

	s = start;
	while (nsects)
	{
		ft_memcpy(info->stab[info->sections++], s->sectname, 16);
		s = ((void*)s + sizeof(struct section_64));
		nsects--;
	}
}

int			ft_nm_init(t_nm *a, t_nm32 *b)
{
	ft_memset(a, 0, sizeof(struct s_nm));
	ft_memset(b, 0, sizeof(struct s_nm32));
	return (1);
}
