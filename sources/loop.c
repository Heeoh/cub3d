/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: insub <insub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 19:18:56 by inskim            #+#    #+#             */
/*   Updated: 2023/07/29 22:32:43 by insub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/my_types.h"
#include "../headers/drawing_3d.h"
#include "../library/mlx/mlx.h"

//test 용
void    make_img(t_game *game_info);
void    draw_map(t_player player, t_img img, t_map map);
void    draw_floor_ceil(t_game *game_info, int floor_color, int ceil_color);
void    print_img(t_game *game_info);
void	__test_init(t_game *game_info);

/* destroy_game
 * : 리소스 해제
 *
 * parameter - game_info: 게임 정보
 * return: none
 */
void	destroy_game(t_game *game_info){
	//리소스 해제.
	game_info++;
}

/* handle_close
 * : ESC, redcross 입력 시 호출되는 함수
 *
 * parameter - game_info: 게임 정보 
 * return: int
 */
int	handle_close(t_game *game_info){
	destroy_game(game_info);
	exit(0);
}

/* move_dir
 * : <- , -> 화살표 좌우를 눌렸을시 플레이어 방향 벡터와 카메라 벡터 방향 전환.  
 *	라디안 = 각도 * 원주율 / 180 임.(공식)
 *	각각의 벡터에 회전 벡터 곱해주면 방향 전환 된 후 벡터를 구할 수 있음.
 *
 * parameter - dir: 누른 화살표
 * 			 - player: 플레이어 방향 벡터와, 카메라 벡터
 * return: none
 */
void	move_dir(enum e_direction dir, t_player *player){
    double	oldDirX;
    double	oldPlaneX;
	double	rot_rate;
	
	if (dir == LEFT)
		rot_rate = ROTATE_DEGREE * PI / 180;
	else
		rot_rate = -ROTATE_DEGREE * PI / 180;
	oldDirX = player->dir.x;
	player->dir.x = player->dir.x * cos(rot_rate) - player->dir.y * sin(rot_rate);
    player->dir.y = oldDirX * sin(rot_rate) + player->dir.y * cos(rot_rate);
	oldPlaneX = player->plane.x;
	player->plane.x = player->plane.x * cos(rot_rate) - player->plane.y * sin(rot_rate);
	player->plane.y = oldPlaneX * sin(rot_rate) + player->plane.y * cos(rot_rate);
}

/* is_not_wall
 * : 플레이어가 이동하려는 위치가 벽이 아닌지 확인함.
 *	플레이어가 이동하려는 위치의 8방향이 모두 벽이 아니면 이동 가능.
 *
 * parameter - board: 맵 정보
 * 			 - x: 플레이어가 이동하려는 x좌표
 * 			 - y: 플레이어가 이동하려는 y좌표
 * return: int
 */
int	is_not_wall(char **board, double x, double y)
{
	double	padding;

	padding = (double)1 / TILESIZE;
	if (board[(int)y][(int)x] != WALL && \
	board[(int)y ][(int)(x + padding)] != WALL && \
	board[(int)y][(int)(x - padding)] != WALL && \
	board[(int)(y - padding)][(int)x] != WALL && \
	board[(int)(y - padding)][(int)(x + padding)] != WALL && \
	board[(int)(y - padding)][(int)(x - padding)] != WALL && \
	board[(int)(y + padding)][(int)x] != WALL && \
	board[(int)(y + padding)][(int)(x + padding)] != WALL && \
	board[(int)(y + padding)][(int)(x - padding)] != WALL)
		return true;
	return false;
}

/* move_player
 * : WASD를 눌렸을시 플레이어 위치를 방향벡터에 따라 이동시킴.
 *	atan2(기울기) -> 라디안임.
 *	라디안을 알면 sin cos를 이용하여 x,y 구할 수 있음.
 *	up down 은 방향 벡터로, left right은 카메라 벡터를 이용함(방향벡터와 수직이므로).
 * 	down, left의 경우 반대로 가야하니 음의 방향으로 이동함.
 * 
 * parameter - dir: WASD 키 
 * 			 - game_info: 게임 정보
 * return: none
 */
void	move_player(enum e_direction dir, t_game *game_info){
	double	radian;
	double	x;
	double	y;

	if (dir == UP || dir == DOWN)//dir 방향일때,
		radian = atan2(-game_info->player.dir.y, game_info->player.dir.x);
	else //plane 방향일때,
		radian = atan2(-game_info->player.plane.y, game_info->player.plane.x);
	x = cos(radian) * ((double)1 / TILESIZE);
	y = sin(radian) * ((double)1 / TILESIZE);
	if (dir == DOWN || dir == LEFT)// down 이나 left면  방향 반대이므로 -1 곱함.
	{
		x *= -1;
		y *= -1;
	}
	x += game_info->player.loc.x;
	y += game_info->player.loc.y;

	if (is_not_wall(game_info->map.board, x, y))
	{
		game_info->player.loc.x = x;
		game_info->player.loc.y = y;
	}
}

/* handle_key
 * : mlx에 등록되어 mlx_loop 도중 키보드 입력시 대응되는 키보드 값을 처리함.
 *   플레이어 이동 혹은 시선 이동임. 변경이 되면 새로운 화면 출력
 *
 * parameter - keycode: 입력 받은 키보드 값
 * 			 - game_info: 게임 정보
 * return: int (왜 인지는 모르겠음.)
 */
int	handle_key(int keycode, t_game *game_info){
	if (keycode == KEY_LEFT)
		move_dir(LEFT, &game_info->player);
	else if (keycode == KEY_RIGHT)
		move_dir(RIGHT, &game_info->player);
	else if (keycode == KEY_W)
		move_player(UP, game_info);
	else if (keycode == KEY_S)
		move_player(DOWN, game_info);
	else if (keycode == KEY_A)
		move_player(LEFT, game_info);
	else if (keycode == KEY_D)
		move_player(RIGHT, game_info);
	else if (keycode == KEY_ESC)
		handle_close(game_info);
	else
		return (0);
	print_img(game_info);
	return (0);
}

int	handle_mouse(t_game *game_info)
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
	else
		print_img(game_info);
	return (0);
}

int	init(char *file_name, t_game *game_info);
int	print_error(int error);

int	main(int argc, char **argv){
	t_game	game_info;


	if (argc != 2)
		return (print_error(ERROR_ARG_NUM));
	if (init(argv[1], &game_info) == ERROR_INT)
		return (0);
	//화면 표시
	print_img(&game_info);
	//hook
	mlx_hook(game_info.win, ON_KEYDOWN, 0, handle_key, &game_info);
	mlx_hook(game_info.win, ON_DESTROY, 0, handle_close, &game_info);
	//loop hook
	mlx_mouse_hide();
	mlx_loop_hook(game_info.mlx, handle_mouse, &game_info);
	mlx_loop(game_info.mlx);
	//리소스 해제
	destroy_game(&game_info);
	return (0);
}
