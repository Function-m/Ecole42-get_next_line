/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suham <suham@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 01:43:08 by suham             #+#    #+#             */
/*   Updated: 2023/07/18 10:18:02 by suham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static int	_initialize(t_gnl_data *data, int fd)
{
	data->read.content = NULL;
	data->read.size = 0;
	data->parsed = NULL;
	if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE < 1 || BUFFER_SIZE > SSIZE_MAX)
		return (GNL_ERROR);
	if ((find_gnl_storage(data, fd) == GNL_ERROR)
		&& (make_gnl_storage(data, fd) == GNL_ERROR))
		return (GNL_ERROR);
	data->read.content = malloc(BUFFER_SIZE);
	if (data->read.content == NULL)
		return (GNL_ERROR);
	return (GNL_SUCCESS);
}

int	_parse_cut(t_gnl_data *data, ssize_t cut_size)
{
	char	*rest;

	data->storage.now->size -= cut_size;
	if (data->storage.now->size == 0)
	{
		data->parsed = data->storage.now->content;
		return (_FREE_STORAGE);
	}
	if (make_dup \
		(&(data->parsed), data->storage.now->content, cut_size) == GNL_ERROR)
		return (_FREE_CONTENT | _FREE_STORAGE);
	if (make_dup(&rest, data->storage.now->content + cut_size, \
		data->storage.now->size) == GNL_ERROR)
		return (_FREE_CONTENT | _FREE_STORAGE | _FREE_PARSED);
	free(data->storage.now->content);
	data->storage.now->content = rest;
	return (GNL_SUCCESS);
}

int	_parse_tie(t_gnl_data *data)
{
	char	*tied;
	ssize_t	i;

	tied = malloc(data->storage.now->size + data->read.size + 1);
	if (tied == NULL)
		return (GNL_ERROR);
	i = 0;
	while (i < data->storage.now->size)
	{
		*(tied + i) = *(data->storage.now->content + i);
		++i;
	}
	i = 0;
	while (i < data->read.size)
	{
		*(tied + data->storage.now->size + i) = *(data->read.content + i);
		++i;
	}
	*(tied + data->storage.now->size + data->read.size) = '\0';
	if (data->storage.now->content != NULL)
		free (data->storage.now->content);
	data->storage.now->content = tied;
	data->storage.now->size += data->read.size;
	return (GNL_SUCCESS);
}

int	_parse(t_gnl_data *data, int fd)
{
	ssize_t	cut_size;

	cut_size = \
		find_chr(data->storage.now->content, data->storage.now->size, '\n');
	if (cut_size != GNL_ERROR)
		return (_parse_cut(data, cut_size));
	while (1)
	{
		data->read.size = read(fd, data->read.content, BUFFER_SIZE);
		if (data->read.size < 0
			|| data->read.size > SSIZE_MAX - data->storage.now->size)
			return (_FREE_CONTENT | _FREE_STORAGE);
		if (data->read.size == 0)
		{
			data->parsed = data->storage.now->content;
			return (_FREE_STORAGE);
		}
		if (_parse_tie(data) == GNL_ERROR)
			return (_FREE_CONTENT | _FREE_STORAGE);
		cut_size = \
			find_chr(data->read.content, data->read.size, '\n');
		if (cut_size != GNL_ERROR)
			return (_parse_cut(data, data->storage.now->size \
				- data->read.size + cut_size));
	}
}

char	*get_next_line(int fd)
{
	int					flag;
	static t_gnl_data	data = {.storage.head = NULL, .storage.now = NULL};

	if (_initialize(&data, fd) == GNL_ERROR)
		flag = _FREE_CONTENT | _FREE_STORAGE;
	else
		flag = _parse(&data, fd);
	if (flag & _FREE_PARSED)
	{
		free(data.parsed);
		data.parsed = NULL;
	}
	if (data.read.content != NULL)
		free(data.read.content);
	if (find_gnl_storage(&data, fd) == GNL_ERROR)
		return (data.parsed);
	if (flag & _FREE_CONTENT)
	{
		free(data.storage.now->content);
		data.storage.now->content = NULL;
		data.storage.now->size = 0;
	}
	if (flag & _FREE_STORAGE)
		drop_gnl_storage(&data, fd);
	return (data.parsed);
}
