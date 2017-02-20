#include "tron.h"
#include "mouse.h"
#include "vars.h"
#include "i8042.h"
#include "timer.h"

#include "kbd.h"
#include "players.h"

#include "mp_local.h"
#include "video_gr.h"
#include "menu.h"
#include "level.h"
#include "scoreboard.h"
#include "singleplayer.h"

extern mouse_t* mouse_info;
extern menu_t *menu;
extern Program *Tron;
extern player *player_1, *player_2;
extern scoreboard_t *scoreboard;
extern timer_struct *timer;

extern unsigned long vram_size;
extern short *double_buffer;
extern short *game_field;



void tick () // UPDATE EVERYTHING
{

	int i;

	switch (Tron->State) //Draw
	{
		case Menu_video:
			
			menu_draw(double_buffer);
			mouse_draw(double_buffer);

			memcpy(getGraphicsBuffer(), double_buffer, vram_size);
			memset(double_buffer, 0, vram_size);

			break;

		case Multiplayer_local:
			if (!Tron->first_game_tick)
			{
				mp_local_tick ();
			}

			level_draw (game_field);

			memcpy(double_buffer, game_field, vram_size);

			scoreboard_draw (double_buffer);
			
			
			player_1_draw (double_buffer);
			player_2_draw (double_buffer);
			

			if (Tron->first_game_tick)
			{
				//Ou desenhar player _ wins e esperar 3 segundos antes de sair para o menu
				if (player_2->wins == 5)
				{
					if (timer->contador_end != 0)
					{
						player_2_win_draw (double_buffer);
						timer->contador_end--;
					}
					else
					{
						tron_change_state (Menu_video);
					}
				}
				else if (player_1->wins == 5)
				{
					if (timer->contador_end != 0)
					{
						player_1_win_draw (double_buffer);
						timer->contador_end--;
					}
					else
					{
						tron_change_state (Menu_video);
					}
				}

				//Esperar 3 segundos e desenhar bitmaps 3 2 1

				else if (timer->contador_start !=0)
				{
					timer_draw (double_buffer);
					timer->contador_start--;
				}
				else
				{
					Tron->first_game_tick=0;
				}
			}

			memcpy(getGraphicsBuffer(), double_buffer, vram_size);
			memset(double_buffer, 0, vram_size);

			if (player_1->crash == 1 && player_2->crash ==1)	//Both players crashed
			{
				memset(game_field, 0, vram_size);
				players_reset ();
				timer->contador_start = 180;
				Tron->first_game_tick = 1;
			}
			else if (player_1->crash == 1)		//Player 1 crashed
			{
				player_2->wins ++;
				memset(game_field, 0, vram_size);
				players_reset ();
				timer->contador_start = 180;
				Tron->first_game_tick = 1;
			}
			else if (player_2->crash == 1)		//Player 2 crashed
			{
				player_1->wins ++;
				memset(game_field, 0, vram_size);
				players_reset ();
				timer->contador_start = 180;
				Tron->first_game_tick = 1;
			}
			break;
		case Singleplayer:
			if (!Tron->first_game_tick)
			{
				single_local_tick ();
			}

			level_draw (game_field);

			memcpy(double_buffer, game_field, vram_size);

			scoreboard_draw (double_buffer);


			player_1_draw (double_buffer);
			player_2_draw (double_buffer);


			if (Tron->first_game_tick)
			{
				//Ou desenhar player _ wins e esperar 3 segundos antes de sair para o menu
				if (player_2->wins == 5)
				{
					if (timer->contador_end != 0)
					{
						player_2_win_draw (double_buffer);
						timer->contador_end--;
					}
					else
					{
						tron_change_state (Menu_video);
					}
				}
				else if (player_1->wins == 5)
				{
					if (timer->contador_end != 0)
					{
						player_1_win_draw (double_buffer);
						timer->contador_end--;
					}
					else
					{
						tron_change_state (Menu_video);
					}
				}

					//Esperar 3 segundos e desenhar bitmaps 3 2 1

				else if (timer->contador_start !=0)
				{
						timer_draw (double_buffer);
						timer->contador_start--;
					}
				else
					{
						Tron->first_game_tick=0;
					}
				}

				memcpy(getGraphicsBuffer(), double_buffer, vram_size);
				memset(double_buffer, 0, vram_size);


				if (player_1->crash == 1 && player_2->crash ==1)	//Both players crashed
				{
					memset(game_field, 0, vram_size);
					players_reset ();
					timer->contador_start = 180;
					Tron->first_game_tick = 1;
				}
				else if (player_1->crash == 1)		//Player 1 crashed
				{
					player_2->wins ++;
					memset(game_field, 0, vram_size);
					players_reset ();
					timer->contador_start = 180;
					Tron->first_game_tick = 1;
				}
				else if (player_2->crash == 1)		//Player 2 crashed
				{
					player_1->wins ++;
					memset(game_field, 0, vram_size);
					players_reset ();
					timer->contador_start = 180;
					Tron->first_game_tick = 1;
				}
			break;
		
		case Instructions:

			drawBitmap(Tron->instructions, 0, 0, ALIGN_LEFT, double_buffer);
			memcpy(getGraphicsBuffer(), double_buffer, vram_size);
			memset(double_buffer, 0, vram_size);

			break;
		default:
			// Do nothing
			break;
	}	


}


void keyboard_event ()
{
	switch (Tron->State)
	{
		case Menu_video:
			switch (Tron->scancode)
			{
				case KEY_W_MAKE:
					menu_change_selection(Up);
					break;

				case KEY_S_MAKE:
					menu_change_selection(Down);
					break;

				case KEY_ENTER_MAKE:
					menu_make_selection();
					break;
				default:
					break;
			}
			break;
		case Multiplayer_local:

			switch (Tron->scancode)
			{
				
				case KEY_W_MAKE:
					player_change_direction (1, Up);
					break;

				case KEY_A_MAKE:
					player_change_direction (1, Left);
					break;

				case KEY_S_MAKE:
					player_change_direction (1, Down);
					break;

				case KEY_D_MAKE:
					player_change_direction (1, Right);
					break;

				case KEY_ARROWUP_MAKE:
					player_change_direction (2, Up);
					break;

				case KEY_ARROWLEFT_MAKE:
					player_change_direction (2, Left);
					break;

				case KEY_ARROWDOWN_MAKE:
					player_change_direction (2, Down);
					break;

				case KEY_ARROWRIGHT_MAKE:
					player_change_direction (2, Right);
					break;
				
				case KEY_ESC_BREAK:
					tron_change_state  (Menu_video);
					break;
				default:
					break;
			}
			break;
		case Singleplayer:
			switch (Tron->scancode)
			{
				case KEY_W_MAKE:
					player_change_direction (1, Up);
					break;

				case KEY_A_MAKE:
					player_change_direction (1, Left);
					break;

				case KEY_S_MAKE:
					player_change_direction (1, Down);
					break;

				case KEY_D_MAKE:
					player_change_direction (1, Right);
					break;
				case KEY_ESC_BREAK:
					tron_change_state (Menu_video);
					break;
				default:
					break;
			}
			break;
		case Instructions:
			switch (Tron->scancode)
			{
				case KEY_ESC_BREAK:
					tron_change_state  (Menu_video);
					break;
				default:
					break;
			}
			break;
		default: 
			break;
	}	

}

void mouse_event ()
{
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	

	int i;

	// Left Button
	if ((Tron->packet[0] & MOUSE_LB) && !mouse_info->lb_pressed)
	{
		mouse_info->lb_pressed = 1;
	}
	else if (!(Tron->packet[0] & MOUSE_LB) && mouse_info->lb_pressed)
	{
		mouse_info->lb_pressed = 0;
	}

	// Middle Button
	if ((Tron->packet[0] & MOUSE_MB) && !mouse_info->mb_pressed)
	{
		mouse_info->mb_pressed = 1;
	}
	else if (!(Tron->packet[0] & MOUSE_MB) && mouse_info->mb_pressed)
	{
		mouse_info->mb_pressed = 0;
	}

	// Right Button
	if ((Tron->packet[0] & MOUSE_RB) && !mouse_info->rb_pressed)
	{
		mouse_info->rb_pressed = 1;
	}
	else if (!(Tron->packet[0] & MOUSE_RB) && mouse_info->rb_pressed)
	{
		mouse_info->rb_pressed = 0;
	}

	if (Tron->packet[0] & MOUSE_X_SIGN)
	{
		if ((mouse_info->mouse_x -(256 - Tron->packet[1])) <= 0)
		{
			mouse_info->mouse_x = 0;
		}
		else
		{
			mouse_info->mouse_x = mouse_info->mouse_x -(256 - Tron->packet[1]);
		}
	}
	else
	{
		if ((mouse_info->mouse_x + Tron->packet[1]) >= (h_res-1))
		{
			mouse_info->mouse_x = h_res;
		}
		else
		{
			mouse_info->mouse_x = mouse_info->mouse_x + Tron->packet[1];
		}
	}

	if (Tron->packet[0] & MOUSE_Y_SIGN)
	{
		if ((mouse_info->mouse_y + (256 - Tron->packet[2]))>=(v_res-1))
		{
			mouse_info->mouse_y = v_res;
		}
		else
		{
			mouse_info->mouse_y = mouse_info->mouse_y + (256 - Tron->packet[2]);
		}
	}
	else
	{
		if ((mouse_info->mouse_y - Tron->packet[2])<=0)
		{
			mouse_info->mouse_y = 0;
		}
		else
		{
			mouse_info->mouse_y = mouse_info->mouse_y - Tron->packet[2];
		}
	}

	switch (Tron->State)
	{
		case Menu_video:
			for (i=0; i<menu->n; i++)
			{
				if ((mouse_info->mouse_x < ((menu->buttons[i]->x_pos)+(menu->buttons[i]->image[0]->bitmapInfoHeader.width/2))) && 
				(mouse_info->mouse_x > ((menu->buttons[i]->x_pos)-(menu->buttons[i]->image[0]->bitmapInfoHeader.width/2))) && 
				(mouse_info->mouse_y < ((menu->buttons[i]->y_pos)+(menu->buttons[i]->image[0]->bitmapInfoHeader.height))) &&
				(mouse_info->mouse_y > ((menu->buttons[i]->y_pos)))
				)
				{
					menu->buttons[menu->selection]->selected = 0;
					menu->selection = i;
					menu->buttons[menu->selection]->selected = 1;
					if (mouse_info->lb_pressed == 1)
					{
						menu_make_selection();
						return;
					}
				}
					
			}
			break;
		case Multiplayer_local:
			break;
		case Instructions:
			break;
		default: 
			break;
	}	
}
