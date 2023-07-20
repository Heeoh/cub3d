/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: insub <insub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 20:37:39 by insub             #+#    #+#             */
/*   Updated: 2023/07/20 20:40:13 by insub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../library/mlx/mlx.h"
#include "../headers/my_types.h"
#include "../headers/raycasting.h"
#include <stdio.h>

void    make_img(t_game *game_info);
void    draw_map(t_player player, t_img img, t_map map);
void    draw_floor_ceil(t_game *game_info, int floor_color, int ceil_color);
void    print_img(t_game *game_info);

/* __test_player_print
 * : player 위치, 방향 벡터, 카메라 벡터 출력
 *
 * parameter - game_info: 게임 정보
 * return: none
 */
void	__test_player_print(t_game *game_info){
	printf("loc\t%lf    %lf\n", game_info->player.loc.x, game_info->player.loc.y);
	printf("dir\t%lf    %lf\n", game_info->player.dir.x, -game_info->player.dir.y);
	printf("plane\t%lf    %lf\n\n", game_info->player.plane.x, -game_info->player.plane.y);	
}

/* __test_init
 * : game_info 초기화
 *
 * parameter - game_info: 게임 정보 
 * return: none
 */
void	__test_init(t_game *game_info){
	//init map
	int width = 15;
	int height = 15;
	static char arrs[15][15] ={
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//0
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//1
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//2
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//3
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//4
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//5
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//6
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//7 x = 13, y = 7
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},//8
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	// static char arrs[14][29] ={
	// 	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	// 	{1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	// 	{1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	// 	{1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	// 	{1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
	// 	{1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1},
	// 	{1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1},
	// 	{1,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,0,1,0,0,0,1},
	// 	{1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1},
	// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
	// 	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1},
	// 	{1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,1},
	// 	{1,1,1,1,0,1,1,1,' ',1,1,1,0,1,0,1,' ',1,0,1,1,1,1,0,0,0,0,0,1},
	// 	{1,1,1,1,1,1,1,1,' ',1,1,1,1,1,1,1,' ',1,1,1,1,1,1,1,1,1,1,1,1}
	// };
	char **board = malloc(sizeof(char *) * height);
	for (int i = 0; i < height; i++){
		board[i] = arrs[i];
	}
	game_info->map.board = board;
	game_info->map.height = height;
	game_info->map.width = width;
	//init player
	game_info->player.loc.x = 3;
	game_info->player.loc.y = 3;
	game_info->player.dir.x = -1;
	game_info->player.dir.y = 0;
	game_info->player.plane.x = 0;
	game_info->player.plane.y = 0.66;
}

/* __test_raycasting
 * : do_raycasting 결과 값 출력
 *
 * parameter - game_info: 게임 정보 
 * return: none
 */
void	__test_raycasting(t_game *game_info){
	double arr[WIN_WIDTH] = {0,};
	for (int i = 0; i < WIN_WIDTH; i++)
		arr[i] = 0;
	double *dist_of_rays = arr;
	do_raycasting(&dist_of_rays, game_info->player, WIN_WIDTH, game_info->map.board);	
}