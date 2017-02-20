#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "test3.h"

static int keyboard_hook = 2;

int kbd_subscribe_int() 
{
	/*Subscribes and enables the keyboard interrupts*/

	int IN_BIT = keyboard_hook;
	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook) != 0 || sys_irqenable(&keyboard_hook) != 0)
		return ERROR;

	return BIT(IN_BIT);
}


int kbd_unsubscribe_int() 
{

	/*Unsubscribes the keyboard interrupts*/

	if(sys_irqdisable(&keyboard_hook) != OK || sys_irqrmpolicy(&keyboard_hook) != OK)
		return ERROR;
	return OK;
}

int kbc_read(unsigned char *data)
{
	unsigned char status;
	int cont = 0;

	while(cont <5)
	{
		if (sys_inb(STAT_REG,(unsigned long*) &status) != OK)
		{
			printf ("ERROR reading status from Status register\n");
			return ERROR;
		}

		if(status & OBF) //Check to see if we can read from OBF
		{
			if(sys_inb(OUT_BUF,(unsigned long*) data) != OK)
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

int kbc_write(unsigned char data)
{
	unsigned char status;
	int cont = 0;

	while(cont < 5)
	{
		if (sys_inb(STAT_REG,(unsigned long*) &status) != OK)
		{
			printf ("ERROR reading status from Status register\n");
			return ERROR;
		}
		if((status & IBF) == 0)
		{
			if(sys_outb(IN_BUF, data) != OK)
			{
				printf ("ERROR writing IN_BUF\n");
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

int kbc_write_cmd (unsigned char cmd, unsigned char args)
{
	unsigned long response = 0;
	int cont;
	for (cont=0; cont < 5; cont++)
	{

		if (kbc_write(cmd) != OK) 
			return ERROR;

		tickdelay(micros_to_ticks(DELAY_US));

		if (sys_inb(OUT_BUF, &response)!=OK)
		{
			printf ("ERRO ao ler resposta\n");
			return ERROR;
		}

		if (response != RSP_ACK) 
			continue;

		if ((cmd == CMD_LED) || (cmd == CMD_RATE_DELAY)) //Does the command ask for arguments?
		{
			while (response != RSP_ERROR)
			{
				if (kbc_write(args) != OK) 
					return ERROR;

				tickdelay(micros_to_ticks(DELAY_US));

				if(sys_inb(OUT_BUF, &response)!=OK)
				{
					printf ("ERRO ao ler resposta\n");
					return ERROR;
				}

				if (response == RSP_ACK)
					return OK;
			}
		}
	}
	return ERROR;
}


unsigned short kbd_int_handler_c () //scancodes can be 2 bytes long
{
	unsigned char data; // we only read 1 byte at a time from the OUT_BUF

	if (kbc_read (&data) != OK)
		return ERROR;
	return data;
}


int kbd_test_scan() 
{
	int ipc_status, r;
	unsigned char irq_kbd = kbd_subscribe_int ();
	unsigned short scancode = 0;
	message msg;

	// kbd_subscribe_int() returns a ERROR value if unsuccessful
	if (irq_kbd == ERROR)
	{
		printf ("kbd_subscribe_int returned ERROR value\n");
		return irq_kbd;
	}
	while(scancode != KEY_ESC_BREAK)
	{
	    /* Get a request message. */
    	if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 )
    	{
       		printf("driver_receive failed with: %d", r);
	   		continue;
    	}

    	if (is_ipc_notify(ipc_status))
    	{ /* received notification */
        	switch (_ENDPOINT_P(msg.m_source))
        	{
            	case HARDWARE: /* hardware interrupt notification */

                	if (msg.NOTIFY_ARG & irq_kbd) /* subscribed interrupt */
                	{
                		if (scancode != SPECIAL_KEY)
							scancode = 0;
						else
							scancode = scancode << 8;

						scancode = kbd_int_handler_c();

						if (scancode != SPECIAL_KEY)
                		{
              				if (scancode & BIT(7))
								printf ("Breakcode : 0x%X \n", scancode);
							else 
								printf ("Makecode : 0x%X \n", scancode);
						}
						else
							scancode = scancode << 8;
							
                	}
                	break;
            	default:
                	break; /* no other notifications expected: do nothing */	
        	}
    	}
    	else 
    	{
    	/* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    	}
	}

	printf ("Pressed ESC\n");
	
	if (kbd_unsubscribe_int() != OK)
	{
		printf("kbd_unsubscribe_int returned ERROR value\n");
		return ERROR;
	}

	return OK;
}
