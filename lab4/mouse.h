#ifndef __MOUSE_H
#define __MOUSE_H

#include "test4.h"


static int mouse_hook = 7;
static int length_drawn = 0;

#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define MOUSE_UNUSED BIT(3)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)

int mouse_write(unsigned char adress, unsigned char data);
int mouse_read(unsigned char adress, unsigned char *data);
void mouse_print(unsigned char *packet);
int mouse_int_handler(unsigned char *packet, int i);
int mouse_subscribe_int();
int mouse_unsubscrib_int();
void moore_machine (unsigned char *packet, state_t *st, short length);
int empty_OUT_BUF();

#endif
