#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "vars.h"


#include "i8042.h"
#include "kbd.h"

static int kbd_hook = 2;

int kbd_subscribe_int()
{

	int IN_BIT = kbd_hook;
	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook) != 0 ||
	 sys_irqenable(&kbd_hook) != 0)
		return ERROR;

	return BIT(IN_BIT);

}

int kbd_unsubscribe_int()
{

	if(sys_irqdisable(&kbd_hook) != OK || sys_irqrmpolicy(&kbd_hook) != OK)
		return ERROR;
	return OK;

}

int kbc_read(unsigned long *data)
{
	unsigned long status;
	int cont = 0;

	while(cont <5)
	{
		if (sys_inb(STAT_REG, &status) != OK)
		{
			printf ("ERROR reading status from Status register\n");
			return ERROR;
		}

		if(status & OBF)
		{
			if(sys_inb(OUT_BUF, data) != OK)
			{
				printf ("ERROR reading from OUT_BUF\n");
				return ERROR;
			}

			if((status & (PAR_ERR | TO_ERR)))
			{
				printf ("OUT_BUF data is corrupted\n");
				return ERROR;
			}
			else
				return OK;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		cont++;
	}
	return ERROR;

}

unsigned short kbd_int_handler ()
{
	unsigned long data;
	static unsigned long scancode = 0;

	if (scancode != SPECIAL_KEY)
	{
		scancode = 0;
	}

	if (kbc_read (&data) != OK)
	{
		printf ("kbd_int_handler:: Error reading scancode\n");
		return ERROR;
	}

	if (data == SPECIAL_KEY)
	{
		scancode = data;
		return (unsigned short)scancode;
	}

	if (scancode == SPECIAL_KEY)
	{
		scancode = scancode << 8;
		scancode |= data;
		return (unsigned short)scancode;
	}

	return (unsigned short)data;
}