/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suham <suham@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 01:43:10 by suham             #+#    #+#             */
/*   Updated: 2023/07/18 10:38:48 by suham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_gnl_data_storage
{
	int								fd;
	char							*content;
	ssize_t							size;
	struct s_gnl_data_storage		*next;
}	t_gnl_data_storage;

typedef struct s_gnl_data
{
	char							*parsed;
	struct s_storage
	{
		struct s_gnl_data_storage	*head;
		struct s_gnl_data_storage	*now;
	}	storage;
	struct s_read
	{
		char						*content;
		ssize_t						size;
	}	read;
}	t_gnl_data;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	0xFFFFFFFF
# endif

# define GNL_ERROR		-1
# define GNL_SUCCESS	0

# define _FREE_PARSED	0b100
# define _FREE_STORAGE	0b10
# define _FREE_CONTENT	0b1

// get_next_line_utils
int		find_gnl_storage(t_gnl_data *data, int fd);
int		make_gnl_storage(t_gnl_data *data, int fd);
int		drop_gnl_storage(t_gnl_data *data, int fd);
int		make_dup(char **new, char *src, ssize_t src_sz);
ssize_t	find_chr(char *dst, ssize_t dst_sz, int chr);

// get_next_line
char	*get_next_line(int fd);

#endif