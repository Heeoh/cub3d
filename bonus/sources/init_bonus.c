/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heson <heson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 17:48:23 by inskim            #+#    #+#             */
/*   Updated: 2023/08/11 13:35:04 by heson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../library/mlx/mlx.h"
#include "../headers/cub3d_bonus.h"
#include "../headers/get_next_line_bonus.h"

static int	is_valid_char(char c)
{
	if (c == '\n' || c == 'F' || c == 'C' || c == 'D' || \
			c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (TRUE);
	return (FALSE);
}

static t_list	*read_file(int fd)
{
	char	*line;
	t_list	*list;
	t_list	*tmp;
	int		is_eof;

	is_eof = 0;
	list = 0;
	line = get_next_line(fd, &is_eof);
	while (line != ERROR_POINTER)
	{
		tmp = ft_lstnew(line);
		if (tmp == ERROR_POINTER)
			break ;
		ft_lstadd_back(&list, tmp);
		line = get_next_line(fd, &is_eof);
	}
	if (!is_eof || !tmp)
	{
		ft_lstclear(&list, free);
		print_error(ERROR_MALLOC, ERROR_INT);
		return (ERROR_POINTER);
	}
	if (!list)
		print_error(ERROR_INVALID_MAP, ERROR_INT);
	return (list);
}

static int	is_texture_set(t_map *map)
{
	if (map->ceil_color == ERROR_INT || map->floor_color == ERROR_INT)
		return (print_error(ERROR_COLOR_NOT_SET, ERROR_INT));
	if (map->north_texture.img == ERROR_POINTER || \
			map->south_texture.img == ERROR_POINTER || \
			map->west_texture.img == ERROR_POINTER || \
			map->east_texture.img == ERROR_POINTER)
		return (print_error(ERROR_TEXTURE_NOT_SET, ERROR_INT));
	return (TRUE);
}

static int	set_map_info(t_list *list, t_map *map, void *mlx, t_game *game_info)
{
	char	*s;

	map->ceil_color = ERROR_INT;
	map->floor_color = ERROR_INT;
	while (list)
	{
		s = list->content;
		while (*s && *s == ' ')
			s++;
		if (*s == '1')
			break ;
		list = list->next;
		if ((*s == 'N' || *s == 'S' || *s == 'W' || *s == 'E' || *s == 'D') \
				&& set_texture(s, mlx, map) == ERROR_INT)
			return (ERROR_INT);
		else if ((*s == 'F' || *s == 'C') && set_color(s, map) == ERROR_INT)
			return (ERROR_INT);
		else if (*s && !is_valid_char(*s))
			return (print_error(ERROR_IMPOSSIBLE_CHAR, ERROR_INT));
	}
	if (is_texture_set(map) == ERROR_INT || \
			set_width_height(list, map) == ERROR_INT || \
			map_parsing(list, game_info) == ERROR_INT)
		return (ERROR_INT);
	return (TRUE);
}

int	init(char *file_name, t_game *game_info)
{
	int		fd;
	t_list	*list;

	ft_memset(game_info, 0, sizeof(t_game));
	if (check_file_name(file_name) == ERROR_INT)
		return (print_error(ERROR_MAP_NAME, ERROR_INT));
	game_info->mlx = mlx_init();
	fd = open(file_name, O_RDONLY, 644);
	if (fd == -1)
		return (print_error(ERROR_OPEN, ERROR_INT));
	list = read_file(fd);
	if (list == ERROR_POINTER)
		return (ERROR_INT);
	if (set_map_info(list, &game_info->map, \
				game_info->mlx, game_info) == ERROR_INT)
	{
		ft_lstclear(&list, free);
		return (ERROR_INT);
	}
	ft_lstclear(&list, free);
	game_info->win = mlx_new_window(game_info->mlx, \
			WIN_WIDTH, WIN_HEIGHT, "Cub3d");
	return (TRUE);
}
