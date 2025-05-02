/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suham <suham@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 01:43:13 by suham             #+#    #+#             */
/*   Updated: 2023/07/18 10:38:19 by suham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	find_gnl_storage(t_gnl_data *data, int fd)
{
	t_gnl_data_storage	*node;

	if (data->storage.now != NULL && data->storage.now->fd == fd)
		return (GNL_SUCCESS);
	node = data->storage.head;
	while (1)
	{
		if (node == NULL)
			return (GNL_ERROR);
		if (node->fd == fd)
		{
			data->storage.now = node;
			return (GNL_SUCCESS);
		}
		node = node->next;
	}
}

int	make_gnl_storage(t_gnl_data *data, int fd)
{
	t_gnl_data_storage	*node;

	data->storage.now = malloc(sizeof(t_gnl_data_storage));
	if (data->storage.now == NULL)
		return (GNL_ERROR);
	data->storage.now->fd = fd;
	data->storage.now->content = NULL;
	data->storage.now->size = 0;
	data->storage.now->next = NULL;
	if (data->storage.head == NULL)
		data->storage.head = data->storage.now;
	else
	{
		node = data->storage.head;
		while (node->next != NULL)
			node = node->next;
		node->next = data->storage.now;
	}
	return (GNL_SUCCESS);
}

int	drop_gnl_storage(t_gnl_data *data, int fd)
{
	t_gnl_data_storage	*node;
	t_gnl_data_storage	*pre_node;

	node = data->storage.head;
	while (1)
	{
		if (node == NULL)
			return (GNL_ERROR);
		if (node->fd == fd)
		{
			if (node == data->storage.head)
				data->storage.head = node->next;
			else
				pre_node->next = node->next;
			if (node == data->storage.now)
				data->storage.now = NULL;
			free(node);
			return (GNL_SUCCESS);
		}
		pre_node = node;
		node = node->next;
	}
}

// return a size until chr
ssize_t	find_chr(char *dst, ssize_t dst_sz, int chr)
{
	ssize_t	i;

	if (dst == NULL)
		return (GNL_ERROR);
	i = 0;
	while (dst_sz != i)
	{
		if (*(dst + i++) == chr)
			return (i);
	}
	return (GNL_ERROR);
}

int	make_dup(char **new, char *src, ssize_t src_sz)
{
	ssize_t	i;

	*new = malloc(src_sz + 1);
	if (*new == NULL)
		return (GNL_ERROR);
	i = 0;
	while (src_sz > i)
	{
		*(*new + i) = *(src + i);
		++i;
	}
	*(*new + src_sz) = '\0';
	return (GNL_SUCCESS);
}
