#ifndef __TIMER_H
#define __TIMER_H

#include "bitmap.h"

/**
 * @brief Timer_struct struct
 */

typedef struct 
{
	Bitmap *time_image[3];
	int contador_start;
	int contador_end;

}timer_struct;

timer_struct *timer;

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int();

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void timer_int_handler();

/**
 * @brief Tests Timer 0 interrupt handling
 *
 * Subscribes Timer 0 interrupts and prints a message once
 *  per second for the specified time interval
 *
 * @param time Length of time interval while interrupts are subscribed
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_test_int(unsigned long time);

/**
 * @brief Allocates memory and initializes timer
 */

void timer_create ();

/**
 * @brief Draws the countdown before every game starts
 *
 * @param buffer in wich to draw
 */

void timer_draw (short *buffer);

/**
 * @brief Destroys the timer and frees the resources used by it
 */

void timer_destroy ();

#endif /* __TIMER_H */
