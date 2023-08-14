/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heson <heson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 19:18:56 by inskim            #+#    #+#             */
/*   Updated: 2023/08/14 16:50:57 by heson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/cub3d_bonus.h"
#include "../../library/mlx/mlx.h"

static void	destroy_game(t_game *game)
{
	if (game->mlx && game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx && game->img.img)
		mlx_destroy_image(game->mlx, game->img.img);
	if (game->mlx && game->map.north_texture.img)
		mlx_destroy_image(game->mlx, game->map.north_texture.img);
	if (game->mlx && game->map.south_texture.img)
		mlx_destroy_image(game->mlx, game->map.south_texture.img);
	if (game->mlx && game->map.west_texture.img)
		mlx_destroy_image(game->mlx, game->map.west_texture.img);
	if (game->mlx && game->map.east_texture.img)
		mlx_destroy_image(game->mlx, game->map.east_texture.img);
	if (game->mlx && game->map.door_texture.img)
		mlx_destroy_image(game->mlx, game->map.door_texture.img);
	free_map(&(game->map.board), game->map.height);
	free_timer(&(game->map.door_timer), game->map.height);
}

static int	handle_close(t_game *game_info)
{
	destroy_game(game_info);
	exit(0);
}

static int	handle_key(int keycode, t_game *game)
{
	if (keycode == KEY_LEFT)
		move_dir(LEFT, &game->player);
	else if (keycode == KEY_RIGHT)
		move_dir(RIGHT, &game->player);
	else if (keycode == KEY_W)
		move_player(UP, game, game->map.board, game->player.loc);
	else if (keycode == KEY_S)
		move_player(DOWN, game, game->map.board, game->player.loc);
	else if (keycode == KEY_A)
		move_player(LEFT, game, game->map.board, game->player.loc);
	else if (keycode == KEY_D)
		move_player(RIGHT, game, game->map.board, game->player.loc);
	else if (keycode == KEY_ESC)
		handle_close(game);
	else if (keycode == KEY_SPACE)
		handle_door(game);
	return (0);
}

static int	handle_frame(t_game *game_info)
{
	int	x;
	int	y;

	mlx_mouse_get_pos(game_info->win, &x, &y);
	if (x < 0 || x > WIN_WIDTH || y < 0 || y > WIN_HEIGHT)
		return (0);
	if (x < WIN_WIDTH / 2 - (WIN_WIDTH / 5))
		handle_key(KEY_LEFT, game_info);
	else if (x > WIN_WIDTH / 2 + (WIN_WIDTH / 5))
		handle_key(KEY_RIGHT, game_info);
	if (game_info->sprite < SPRITE_NUM)
	{
		game_info->player.loc.x += game_info->player.move_offset.x;
		game_info->player.loc.y += game_info->player.move_offset.y;
		game_info->sprite++;
	}
	run_door_timer(game_info);
	print_img(game_info);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game_info;

	if (argc != 2)
		return (print_error(ERROR_ARG_NUM, ERROR_INT));
	if (init(argv[1], &game_info) == ERROR_INT)
	{
		destroy_game(&game_info);
		exit(1);
	}
	if (init_door_timer(&game_info) == ERROR_INT)
	{
		destroy_game(&game_info);
		print_error(ERROR_MALLOC, ERROR_INT);
		exit(1);
	}
	print_img(&game_info);
	mlx_hook(game_info.win, ON_KEYDOWN, 0, handle_key, &game_info);
	mlx_hook(game_info.win, ON_DESTROY, 0, handle_close, &game_info);
	mlx_mouse_hide();
	mlx_loop_hook(game_info.mlx, handle_frame, &game_info);
	mlx_loop(game_info.mlx);
	return (0);
}
