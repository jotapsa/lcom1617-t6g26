#include <minix/syslib.h>
#include <minix/drivers.h>
#include "tron.h"
#include "i8042.h"
#include "vars.h"
#include "mouse.h"
#include "menu.h"
#include "bitmap.h"

extern mouse_t *mouse_info;
extern Program *Tron;
extern menu_t *menu;

static int mouse_hook = 3;


int mouse_read(unsigned char adress, unsigned char *data) 
{
	int i;
	unsigned char status;
	for (i=0; i<5; i++)
	{
		if (sys_inb(STAT_REG,(unsigned long*) &status) != OK){
			printf ("ERROR reading status from Status register\n");
			return ERROR;
		}

		if(!(status & (!OBF | PAR_ERR | TO_ERR) == 0))
		{
			if(sys_inb(adress,(unsigned long*) data) != OK)
			{
				printf ("ERROR reading from OUT_BUF\n");
				return ERROR;
			}
			return OK;
		}
	tickdelay(micros_to_ticks(DELAY_US)); 
	}
	return ERROR;
}


int mouse_write (unsigned char adress, unsigned char data)
{
	unsigned char status;
	int i;

	for(i=0; i<5; i++)
	{
		if (sys_inb(STAT_REG,(unsigned long*) &status) != OK){
			printf ("ERROR reading status from Status register\n");
			return ERROR;
		}
		if((status & IBF) == 0) // can we write ?
		{
			if (sys_outb(adress, data) != OK) 
				return ERROR;
			return OK;
		}
	tickdelay(micros_to_ticks(DELAY_US)); 
	}
	return ERROR;
}


int mouse_subscribe_int()
{
	int IN_BIT = mouse_hook;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook) != OK || sys_irqenable(&mouse_hook) != OK)
		return ERROR;

	return BIT(IN_BIT);
}


int mouse_unsubscribe_int()
{
	if(sys_irqdisable(&mouse_hook) != OK || sys_irqrmpolicy(&mouse_hook) != OK)
		return ERROR;
	return OK;
}

int mouse_int_handler(unsigned char *packet, int counter)
{
	unsigned char tmp;

	if (mouse_read (OUT_BUF, &tmp) != OK)
	{
		printf ("Error reading OUT_BUF");
		return ERROR;
	}
	packet[counter]= (unsigned char) tmp;
	return OK;
}

int empty_OUT_BUF ()
{
	unsigned char status;
	int i;

	for (i=0; i<5; i++)
	{
		if (sys_inb(STAT_REG,(unsigned long*) &status) != OK){
			printf ("ERROR reading status from Status register\n");
			return ERROR;
		}

		tickdelay(micros_to_ticks(DELAY_US)); 

		while (status & (OBF) !=0) //means OBF is full
		{
			unsigned long tmp;
			if (sys_inb(OUT_BUF, &tmp) != OK)
			{
				printf ("ERROR cleaning OUT_BUF\n");
				return ERROR;
			}
			if (sys_inb(STAT_REG,(unsigned long*) &status) != OK)
			{
				printf ("ERROR reading status from Status register\n");
				return ERROR;
			}
			tickdelay(micros_to_ticks(DELAY_US)); 
		}
	}
	if (status & (OBF) !=0)
		return ERROR;
	else 
		return OK;
}

void mouse_create()
{
	unsigned short h_res, v_res;

	h_res = getHorResolution();
	v_res = getVerResolution();	

	mouse_info = (mouse_t*) malloc(sizeof(mouse_t));

	mouse_info->mouse_x = (h_res/2);
	mouse_info->mouse_y = (v_res/2);

	mouse_info->lb_pressed = 0;
	mouse_info->mb_pressed = 0;
	mouse_info->rb_pressed = 0;
	
	mouse_info->cursor = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/mouse_cursor.bmp");
}

void mouse_draw (short *buffer)
{
	drawBitmap_greenscreen(mouse_info->cursor, mouse_info->mouse_x, mouse_info->mouse_y, ALIGN_LEFT, buffer);
}

void mouse_destroy ()
{
	if (mouse_info == NULL)
	{
		return;
	}

	deleteBitmap(mouse_info->cursor);

	free(mouse_info);
}
