#ifndef __MENU_H
#define __MENU_H

#include "bitmap.h"
#include "vars.h"

/**
 * @brief Button struct
 */

typedef struct 
{
	char *name;
	Bitmap *image[2];
	int x_pos, y_pos;
	int selected;
}button;

/**
 * @brief Menu struct
 */

typedef struct 
{
	int n;
	button **buttons;
	unsigned short selection;
	Bitmap *background;

}menu_t;

menu_t *menu;

/**
 * @brief Calls start_menu as well as button_create to generate a menu
 */

void start_menu ();

/**
 * @brief Alocates memory for the menu
 */

void menu_create ();

/**
 * @brief Alocates memory for a button, initializes it and adds it to the menu
 * @param button_name string button name
 * @param bmp_normal Bitmap* when the button is not selected
 * @param bmp_selected Bitmap* when the button is selected
 * @param x x coord 
 * @param y y coord
 */

void button_create (char *button_name, Bitmap *bmp_normal, Bitmap *bmp_selected, int x, int y);

/**
 * @brief Changes menu selection
 * @param direction Direction to change the selection
 */

void menu_change_selection (Direction direction);

/**
 * @brief Code to run when we make a selection in the menu
 */

void menu_make_selection ();

/**
 * @brief Draw a button
 * @param image Bitmap* image to be drawn
 * @param buffer buffer* in wich to draw
 * @param x x coord
 * @param y y coord 
 */

void button_draw(Bitmap *image, short* buffer, int x, int y);

/**
 * @brief Draw the menu
 * @param buffer buffer* in wich to draw
 */

void menu_draw (short* buffer);

/**
 * @brief Destroys the menu, freeing the resources used by it
 */

void menu_destroy ();


#endif
