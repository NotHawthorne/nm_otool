/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmotool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 15:23:41 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/09 04:06:47 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NMOTOOL_H
# define NMOTOOL_H

# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <fcntl.h>
# include <mach-o/loader.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <errno.h>
# include <stdlib.h>
# include "libft.h"

typedef struct				s_nm
{
	struct stat				stat;
	struct mach_header_64	*header;
	char					stab[64][16];
	size_t					sections;
	void					*symtab_start;
	struct s_nm_symbol		*symbols;
	int						is64;
}							t_nm;

typedef struct				s_nm32
{
	struct stat				stat;
	struct mach_header		*header;
	char					stab[24][16];
	size_t					sections;
	void					*symtab_start;
	struct s_nm_symbol		*symbols;
	int						is64;
}							t_nm32;

typedef struct				s_nm_symbol
{
	char					*name;
	uint64_t				val;
	char					type;
	struct s_nm_symbol		*l;
	struct s_nm_symbol		*r;
	int						undef;
}							t_nm_symbol;

void						*nmo_load(char *file, struct stat *s, int prog);
void						nm_print(t_nm_symbol *cur, int is32);
int							ft_nm(char *file);
int							ft_nm32(t_nm32 *h);
int							ft_otool(char *file);
int							ft_otool32(char *file, t_nm32 *h);
void						*nmo_find_header(void *start);

int32_t						switch_endian32(int32_t num);
void						nm_section_it(void *start, int nsects, t_nm *info);
int							ft_nm_init(t_nm *a, t_nm32 *b);
int							nm_symbols(void *start, struct mach_header_64 *h,
										t_nm *info);
int							nm_dump(void *start, t_nm *info, int depth);
int							nm_dump32(void *start, t_nm32 *info, int depth,
										int *os);
int							nm_symbols32(void *start, struct mach_header *h,
										t_nm32 *info);

#endif
