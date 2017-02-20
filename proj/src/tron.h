#ifndef __TRON_H
#define __TRON_H

#define VIDEO_MODE 0x117

#include "bitmap.h"

/**
 * @brief Enum for game states
 */

typedef enum 
{
	Menu_video,
	Multiplayer_local,
	Singleplayer,
	Instructions
}Game_state;

/**
 * @brief Program struct
 */

typedef struct 
{
	int irq_timer, irq_mouse, irq_kbd, irq_rtc, irq_serial; // interrupt bitmasks
	unsigned short scancode;
	unsigned char packet[3];
	int packet_counter;
	Bitmap* instructions;
	int first_game_tick;
	int quit;
	int video_mode;
	Game_state State;
}Program;

Program *Tron;

 /**
 * @brief Exits program after destroying everything
 */

void tron_quit ();

 /**
 * @brief Boots program into the menu
 */

void tron_boot ();

 /**
 * @brief Deletes current state
 */

void tron_delete_state ();

 /**
 * @brief Changes game state
 * @param new_state Game_state to change into
 */

void tron_change_state (Game_state new_state);

int main ();


#endif
