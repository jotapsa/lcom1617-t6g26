#include "players.h"
#include "vars.h"
#include "video_gr.h"
#include "tron.h"
#include "mp_local.h"
#include "level.h"
#include "timer.h"

extern Program *Tron;
extern player *player_1, *player_2;
extern unsigned long vram_size;
extern short *game_field;


void start_mp_local ()
{
	timer_create ();
	level_create ();
	scoreboard_create ();

	players_create ();
	game_field = calloc(vram_size, 1);
}

void mp_local_tick ()
{

	char direcoes [4][20] = {
		"UP",
		"LEFT",
		"RIGHT",
		"DOWN",
	};

	//update positions

	update_player_1 ();
	update_player_2 ();

	// draw player trail

	player_1_draw_trail (game_field, rgb(51,219,214));
	player_2_draw_trail (game_field, rgb(255,130,0));
	
	//check colisions
	
	player_1_check_colision ();
	player_2_check_colision ();
}

void end_mp_local()
{
	timer_destroy ();
	level_destroy ();
	scoreboard_destroy ();
	players_destroy ();
	free (game_field);
}
