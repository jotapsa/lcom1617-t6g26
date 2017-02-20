#include <stdio.h>
#include <stdlib.h>

#include "players.h"
#include "vars.h"
#include "video_gr.h"
#include "bitmap.h"
#include "tron.h"
#include "mp_local.h"

extern player *player_1, *player_2;
extern Program *Tron;
extern short *game_field;

//static int up_bound 140

void players_create() //int velocity
{
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();

	player_1 = (player*) malloc(sizeof(player));

	player_1->direction = Right;
	player_1->x_pos = h_res/4;
	player_1->y_pos = v_res/2;
	player_1->crash = 0;
	player_1->velocity = 5;
	player_1->wins =0 ;
	player_1->bike_image[0] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_1/tron_bike_1_up.bmp");
	player_1->bike_image[1] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_1/tron_bike_1_left.bmp");
	player_1->bike_image[2] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_1/tron_bike_1_right.bmp");
	player_1->bike_image[3] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_1/tron_bike_1_down.bmp");
	player_1->win_image = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_1/player_1_wins.bmp");

	player_2 = (player*) malloc(sizeof(player));

	player_2->direction = Left;
	player_2->x_pos = 3*h_res/4;
	player_2->y_pos = v_res/2;
	player_2->crash = 0;
	player_2->velocity = 5;//6;
	player_2->wins =0;
	player_2->bike_image[0] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_2/tron_bike_2_up.bmp");
	player_2->bike_image[1] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_2/tron_bike_2_left.bmp");
	player_2->bike_image[2] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_2/tron_bike_2_right.bmp");
	player_2->bike_image[3] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_2/tron_bike_2_down.bmp");
	player_2->win_image = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/player_2/player_2_wins.bmp");

}

void players_reset ()
{

	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	

	player_1->direction = Right;
	player_1->x_pos = h_res/4;
	player_1->y_pos = v_res/2;
	player_1->crash =0;

	player_2->direction = Left;
	player_2->x_pos = 3*h_res/4;
	player_2->y_pos = v_res/2;
	player_2->crash =0;
}

void player_change_direction (int player, unsigned short dir) //Typecast to unsigned short otherwise the program crashes for some reason
{
	switch (player)
	{	
		case 1:
			switch (player_1->direction)
			{
				case Up:/*fall through*/
				case Down:
					if ((dir == Left) || (dir == Right))
					{
						player_1->direction = dir;	
					}
					break;
				case Left:
				case Right:
					if ((dir == Up) || (dir == Down))
					{
						player_1->direction = dir;
					}
					break;
				default:
					break;
			}
			break;
		case 2:
			switch (player_2->direction)
			{
				case Up:/*fall through*/
				case Down:
					if ((dir == Left) || (dir == Right))
					{
						player_2->direction = dir;	
					}
					break;
				case Left:
				case Right:
					if ((dir == Up) || (dir == Down))
					{
						player_2->direction = dir;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void player_1_check_colision ()
{	
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	
	

	if (*(game_field + ((player_1->y_pos * h_res +player_1->x_pos))) !=0)
	{
		player_1->crash = 1;
		printf ("Player 1 upper left corner hit the trail\n");
		return;
	}
	if (*(game_field + ((player_1->y_pos * h_res +player_1->x_pos + 19))) !=0)
	{
		player_1->crash = 1;
		printf ("Player 1 upper right corner hit the trail\n");
		return;
	}
	if (*(game_field + ((player_1->y_pos + 19) * h_res +player_1->x_pos)) !=0)
	{
		player_1->crash = 1;
		printf ("Player 1 bottom left corner hit the trail\n");
		return;
	}
	if (*(game_field + ((player_1->y_pos + 19) * h_res +player_1->x_pos + 19)) !=0)
	{
		player_1->crash = 1;
		printf ("Player 1 bottom right corner hit the trail\n");
		return;
	}
}

void player_2_check_colision ()
{	
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	

	if (*(game_field + (player_2->y_pos * h_res +player_2->x_pos)) !=0)
	{
		player_2->crash = 1;
		printf ("Player 2 upper left corner hit the trail\n");
		return;
	}
	if (*(game_field + (player_2->y_pos * h_res +player_2->x_pos + 19)) !=0)
	{
		player_2->crash = 1;
		printf ("Player 2 upper right corner hit the trail\n");
		return;
	}
	if (*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos)) !=0)
	{
		player_2->crash = 1;
		printf ("Player 2 bottom left corner hit the trail\n");
		return;
	}
	if (*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos + 19)) !=0)
	{
		player_2->crash = 1;
		printf ("Player 2 bottom right corner hit the trail\n");
		return;
	}

}

void update_player_1 ()
{
	switch (player_1->direction)
	{
		case Up:
			player_1->y_pos = player_1->y_pos - player_1->velocity;		//UP is going down in the sense that lower y coordinates represent a higher position
			break;
		case Down:
			player_1->y_pos = player_1->y_pos + player_1->velocity;
			break;
		case Left:
			player_1->x_pos = player_1->x_pos - player_1->velocity;
			break;
		case Right:
			player_1->x_pos = player_1->x_pos + player_1->velocity;
			break;

		default:
			break;
	}
}

void update_player_2 ()
{
	switch (player_2->direction)
	{
		case Up:
			player_2->y_pos = player_2->y_pos - player_2->velocity;		//UP is going down in the sense that lower y coordinates represent a higher position
			break;
		case Down:
			player_2->y_pos = player_2->y_pos + player_2->velocity;
			break;
		case Left:
			player_2->x_pos = player_2->x_pos - player_2->velocity;
			break;
		case Right:
			player_2->x_pos = player_2->x_pos + player_2->velocity;
			break;
		default:
			break;
	}
}

void player2_AI()
{
	time_t t;
 	srand((unsigned) time(&t));

	unsigned short h_res, v_res;
	int i = rand () %2;

	h_res = getHorResolution();
	v_res = getVerResolution();

	switch (player_2->direction)
	{
		case Up:
			if ((*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos) !=0) ||
				(*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos + 19) !=0))
			{
				switch (i)
				{
					case 0:
						if ((*(game_field + (player_2->y_pos * h_res +player_2->x_pos + 19 +player_2->velocity )) ==0) &&
						(*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos + 19 +player_2->velocity)) ==0))
						{
							player_change_direction(2, Right);
							return;
						}
						else if ((*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos-player_2->velocity)) ==0) &&
						(*(game_field + (player_2->y_pos * h_res +player_2->x_pos-player_2->velocity)) ==0))
						{
							player_change_direction(2, Left);
							return;
						}
						break;

					case 1:
						if ((*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos-player_2->velocity)) ==0) &&
						(*(game_field + (player_2->y_pos * h_res +player_2->x_pos-player_2->velocity)) ==0))
						{
							player_change_direction(2, Left);
							return;
						}
						else if ((*(game_field + (player_2->y_pos * h_res +player_2->x_pos + 19 +player_2->velocity )) ==0) &&
						(*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos + 19 +player_2->velocity)) ==0))
						{
							player_change_direction(2, Right);
							return;
						}
						break;
				}
			}
			break;
		case Down:
			if ((*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos)) !=0) ||
				(*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos + 19)) !=0))
			{
				switch (i)
				{
					case 0:
						if ((*(game_field + (player_2->y_pos * h_res +player_2->x_pos + 19 +player_2->velocity )) ==0) &&
						(*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos + 19+player_2->velocity)) ==0))
						{
							player_change_direction(2, Right);
							return;
						}
						else if ((*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos-player_2->velocity)) ==0) &&
						(*(game_field + (player_2->y_pos * h_res +player_2->x_pos-player_2->velocity)) ==0))
						{
							player_change_direction(2, Left);
							return;
						}
						break;

					case 1:
						if ((*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos-player_2->velocity)) ==0) &&
						(*(game_field + (player_2->y_pos * h_res +player_2->x_pos-player_2->velocity)) ==0))
						{
							player_change_direction(2, Left);
							return;
						}
						else if ((*(game_field + (player_2->y_pos * h_res +player_2->x_pos + 19 +player_2->velocity )) ==0) &&
						(*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos + 19+player_2->velocity)) ==0))
						{
							player_change_direction(2, Right);
							return;
						}
						break;
				}
			}
			break;
		case Left:
			if ((*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos-player_2->velocity)) !=0) ||
				(*(game_field + (player_2->y_pos * h_res +player_2->x_pos-player_2->velocity)) !=0))
			{
				switch (i)
				{
				case 0:
					if ((*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos) ==0) && 
					(*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos + 19) ==0))
					{
						player_change_direction(2, Up);
						return;
					}
					else if ((*(game_field + ((player_2->y_pos + 19+ player_2->velocity) * h_res +player_2->x_pos)) ==0) &&
					(*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos + 19)) ==0))
					{
						player_change_direction(2, Down);
						return;
					}
					break;
				case 1:
					if ((*(game_field + ((player_2->y_pos + 19+ player_2->velocity) * h_res +player_2->x_pos)) ==0) &&
					(*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos + 19)) ==0))
					{
						player_change_direction(2, Down);
						return;
					}
					else if ((*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos) ==0) && 
					(*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos + 19) ==0))
					{
						player_change_direction(2, Up);
						return;
					}
					break;
				}
			}
			break;
		case Right:
			if ((*(game_field + (player_2->y_pos * h_res +player_2->x_pos + 19 +player_2->velocity )) !=0) ||
				 (*(game_field + ((player_2->y_pos + 19) * h_res +player_2->x_pos + 19 +player_2->velocity)) !=0))
			{
				switch (i)
				{
					case 0:
						if ((*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos) ==0) && 
						(*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos + 19) ==0))
						{
							player_change_direction(2, Up);
							return;
						}
						else if ((*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos)) ==0) &&
						(*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos + 19)) ==0))
						{
							player_change_direction(2, Down);
							return;
						}
						break;
					case 1:

						if ((*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos)) ==0) &&
						(*(game_field + ((player_2->y_pos + 19 + player_2->velocity) * h_res +player_2->x_pos + 19)) ==0))
						{
							player_change_direction(2, Down);
							return;
						}

						else if ((*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos) ==0) && 
						(*(game_field + (player_2->y_pos-player_2->velocity) * h_res +player_2->x_pos + 19) ==0))
						{
							player_change_direction(2, Up);
							return;
						}
						break;
				}
			}
			break;
		default:
			break;
	}
}


void player_1_draw_trail (short *buffer, short color)
{
	int i, j;

	for (i=0; i<player_1->velocity; i++)
	{
		switch (player_1->direction)
		{
			case Up:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_1->x_pos + j, player_1->y_pos + 20+i, color, buffer);
				}
				break;
			case Down:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_1->x_pos + j, player_1->y_pos -1-i, color, buffer);
				}
				break;
			case Left:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_1->x_pos + 20+i, player_1->y_pos +j, color, buffer);
				}
				break;
			case Right:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_1->x_pos -1-i, player_1->y_pos +j, color, buffer);
				}
				break;
			default:
				break;
		}
	}

}

void player_2_draw_trail (short *buffer, short color)
{
	int i, j;

	for (i=0; i<player_2->velocity; i++)
	{
		switch (player_2->direction)
		{
			case Up:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_2->x_pos + j, player_2->y_pos + 20+i, color, buffer);
				}
				break;
			case Down:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_2->x_pos + j, player_2->y_pos -1-i, color, buffer);
				}
				break;
			case Left:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_2->x_pos + 20+i, player_2->y_pos +j, color, buffer);
				}
				break;
			case Right:
				for (j=0; j<20; j++)
				{
					vg_draw_pixel (player_2->x_pos -1-i, player_2->y_pos +j, color, buffer);
				}
				break;
			default:
				break;
		}
	}
}

void player_1_draw (short *buffer)
{
	drawBitmap_greenscreen(player_1->bike_image[player_1->direction], player_1->x_pos, player_1->y_pos, ALIGN_LEFT, buffer);
}

void player_2_draw (short *buffer)
{
	drawBitmap_greenscreen(player_2->bike_image[player_2->direction], player_2->x_pos, player_2->y_pos, ALIGN_LEFT, buffer);
}

void player_1_win_draw (short *buffer)
{
	drawBitmap_greenscreen(player_1->win_image, 0, 0, ALIGN_LEFT, buffer);
}

void player_2_win_draw (short *buffer)
{
	drawBitmap_greenscreen(player_2->win_image, 0, 0, ALIGN_LEFT, buffer);
}

void players_destroy ()
{
	int i;

	for (i=0 ;i<4; i++)
	{
		deleteBitmap(player_1->bike_image[i]);
	}

	deleteBitmap (player_1->win_image);

	free (player_1);

	for (i=0 ;i<4; i++)
	{
		deleteBitmap(player_2->bike_image[i]);
	}

	deleteBitmap (player_2->win_image);

	free (player_2);
}
