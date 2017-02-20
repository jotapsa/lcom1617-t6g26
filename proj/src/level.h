#ifndef __LEVEL_H
#define __LEVEL_H

#include "bitmap.h"

typedef struct 
{
	Bitmap *upper_rectangle;
	Bitmap *lower_rectangle;
	Bitmap *right_rectangle;
	Bitmap *left_rectangle;
	
}level_t;

level_t *level_night, *level_day, *level_dawn;

/**
 * @brief Allocates memory for the levels and initializes each, loading respective bitmaps
 *
 */
void level_create ();

/**
 * @brief Draws the level according to the time, morning, day or night
 *
 * @param buffer to draw
 */

void level_draw (short* buffer);

/**
 * @brief Destroys the levels, freeing all resources used
 */

void level_destroy ();

#endif