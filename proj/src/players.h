#ifndef __PLAYERS_H
#define __PLAYERS_H

#include "bitmap.h"
#include "vars.h"

/**
 * @brief player struct
 */

typedef struct 
{
	Direction direction;
	short x_pos, y_pos;
	unsigned short crash;
	short velocity;
	int wins;
	Bitmap *bike_image[4];
	Bitmap *win_image;
}player;

/*Define variable*/
player *player_1, *player_2;

/**
 * @brief Allocates memory for the players and initializes both
 */

void players_create();

/**
 * @brief Resets player position and direction
 */

void players_reset ();

/**
 * @brief Change player direction
 * @param player to apply changes
 * @param dir new player direction
 */

void player_change_direction (int player, unsigned short dir);

/**
 * @brief Checks if player 1 has colided
 */

void player_1_check_colision ();

/**
 * @brief Checks if player 2 has colided
 */

void player_2_check_colision ();

/**
 * @brief Updates player 1 position
 */

void update_player_1 ();

/**
 * @brief Updates player 2 position
 */

void update_player_2 ();

/**
 * @brief player 2 AI
 */


void player2_AI();

/**
 * @brief Draws player 1 trail
 * @param buffer to draw
 * @param color in wich the trail will be drawn
 */

void player_1_draw_trail (short *buffer, short color);

/**
 * @brief Draws player 2 trail
 * @param buffer to draw
 * @param color in wich the trail will be drawn
 */

void player_2_draw_trail (short *buffer, short color);

/**
 * @brief Draws player 1
 * @param buffer to draw
 */

void player_1_draw (short *buffer);

/**
 * @brief Draws player 2
 * @param buffer to draw
 */

void player_2_draw (short *buffer);

/**
 * @brief Draws player 1 win bitmap
 * @param buffer to draw
 */

void player_1_win_draw (short *buffer);

/**
 * @brief Draws player 2 win bitmap
 * @param buffer to draw
 */

void player_2_win_draw (short *buffer);

/**
 * @brief Destroys the players, freeing the resources used
 */

void players_destroy ();

#endif
