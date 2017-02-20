#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

#include "bitmap.h"

/**
 * @brief scoreboard struct
 */

typedef struct 
{
	Bitmap *numbers[10];
	Bitmap *board;
}scoreboard_t;

scoreboard_t *scoreboard;

/**
 * @brief Allocates memory and initializes the scoreboard
 */

void scoreboard_create ();

/**
 * @brief Draws the scoreboard
 *
 * @param buffer in wich to draw
 */

void scoreboard_draw (short *buffer);

/**
 * @brief Destroys the scoreboard, freeing the resources used by it
 */

void scoreboard_destroy ();

#endif