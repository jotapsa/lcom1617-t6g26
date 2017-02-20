#ifndef __MOUSE_H
#define __MOUSE_H

#include "bitmap.h"

#define MOUSE_IRQ 12

#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define MOUSE_UNUSED BIT(3)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)

/**
 * @brief mouse struct
 */

typedef struct
{
	unsigned short mouse_x, mouse_y;

	unsigned char lb_pressed;
	unsigned char mb_pressed;
	unsigned char rb_pressed;

	int size;
	int color1, color2;

	Bitmap *cursor;

}mouse_t;

/*Define variable*/
mouse_t *mouse_info;

/**
 * @brief reads data from the adress
 *
 * @param adress where to read from
 * @param data pointer to store what we read from the adress
 * @return int ERROR or OKAY values, as defined
 */

int mouse_read(unsigned char adress, unsigned char *data);

/**
 * @brief writes in the address some given data
 *
 * @param adress address in wich to write the date
 * @param data  data to write in the adress
 * @return int ERROR or OKAY values, as defined
 */

int mouse_write(unsigned char adress, unsigned char data);

/**
 * @brief Subscribes to mouse interrupts
 * @return int ERROR or OKAY values, as defined
 */

int mouse_subscribe_int();

/**
 * @brief Unsubscribes to mouse interrupts
 * @return int ERROR or OKAY values, as defined
 */

int mouse_unsubscribe_int();

/**
 * @brief reads packet data
 *
 * @param packet array in wich to write the packet info
 * @param i counter for what packet we are receiving
 * @return int ERROR or OKAY values, as defined
 */

int mouse_int_handler(unsigned char *packet, int i);

/**
 * @brief Clears the OUT_BUF
 * @return int ERROR or OKAY values, as defined
 */

int empty_OUT_BUF();

/**
 * @brief Allocates memory and initializes mouse_info
 */

void mouse_create();

/**
 * @brief Draw mouse
 * @param buffer buffer to draw
 */

void mouse_draw (short *buffer);

/**
 * @brief Destroys the mouse and frees the resources used by it
 */

void mouse_destroy ();

#endif
