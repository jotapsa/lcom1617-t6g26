#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "mouse.h"

int mouse_read(unsigned char adress, unsigned char *data) //returns
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
		/*Unsubscribes the mouse interrupts*/
	int i;
	unsigned char response;

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

void mouse_print(unsigned char* packet) {
	
	printf("B1: 0x%X\tB2: 0x%X\tB3: 0x%X\tLB: %d\tMB: %d\tRB: %d\tXOV: %d\tYOV: %d\t", packet[0], packet[1], packet[2], 
		packet[0] & BIT(2) ? 1 : 0,
		packet[0] & BIT(1) ? 1 : 0, 
		packet[0] & BIT(0) ? 1 : 0, 
		packet[0] & BIT(6) ? 1 : 0,
		packet[0] & BIT(7) ? 1 : 0);

	if(packet[0] & BIT(4))
		printf("X:-%d\t", 256 - packet[1]);
	else
		printf("X:%d\t", packet[1]);

	if(packet[0] & BIT(5))
		printf("Y:-%d\t", 256 - packet[2]); 
	else
		printf("Y:%d\t", packet[2]);
	printf ("\n");
}

void moore_machine (unsigned char *packet, state_t *st, short length)
{
	int Y_movement;

	switch(*st)
	{
		case init:
			if (packet[0] & BIT(0)) // pressed right button
			{
				length_drawn =0 ;
				*st=draw;
			}
			break;
		case draw:

			if ((packet[0] & BIT(0)) == 0) // released right button
			{
				*st=init;
			}

			if(packet[0] & BIT(5))
			{
				Y_movement= -(256-packet[2]);
			}
			else
			{
				Y_movement = packet[2];
			}

			if ((Y_movement == 0)|| ((length >0) && (Y_movement <0)) || ((length<0) &&(Y_movement>0))) // stopped movement, or went in the oposite direction, need to reset
			{
				length_drawn = 0;
			}
			else
			{
				length_drawn += Y_movement;
			}

			if (((length <0) && (length_drawn<=length)) || ((length >0) && (length_drawn>=length)))
				*st=comp;
			break;
		case comp:
			break;
		default:
			break;
	}
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







