#include <minix/syslib.h>
#include <minix/com.h>
#include <minix/drivers.h>
#include <stdio.h>

#include "tron.h"
#include "i8042.h"
#include "vars.h"

#include "events.h"
#include "mp_local.h"
#include "menu.h"

#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "rtc.h"
#include "video_gr.h"

#include "singleplayer.h"

extern mouse_t *mouse_info;
extern menu_t *menu;
extern Program *Tron;

void tron_quit ()
{	
	int i;
	unsigned char response=0;

	if (timer_unsubscribe_int() != OK)
	{
		printf ("tron_quit :: Error unsubscribing from timer\n");
		//return ERROR;
	}

	if (kbd_unsubscribe_int () != OK)
	{
		printf ("tron_quit :: Error unsubscribing from keyboard\n");
	}

	//DISABLE DATA REPORTING
	for (i=0, response = 0; ((i<5) && (response != RSP_ACK))  ; i++)
	{
		mouse_write (KBC_CMD_REG, MOUSE_WRITE_CMD);
		mouse_write (IN_BUF, MOUSE_DISB);
		mouse_read (OUT_BUF, &response);
	}
	if (response != RSP_ACK)
	{
		printf ("Timeout disabling data reporting\n");
		//return ERROR;
	}

	if (mouse_unsubscribe_int() != OK)
	{
		printf ("tron_quit:: Error unsubscribing from mouse\n");
	}
	// unsubscribe

	empty_OUT_BUF();

	mouse_destroy (mouse_info);

	printf ("Exited Tron\n");

}

void tron_boot ()
{

	/*synchronize with the RS*/
	sef_startup();

	Tron = (Program*) malloc(sizeof(Tron));

	unsigned char response=0;
	int i;

	if (Tron == NULL)
	{
		tron_quit();
	}

	Tron->irq_kbd = kbd_subscribe_int();

	if (Tron->irq_kbd == ERROR)
	{
		printf ("tron_boot:: Error subscribing to timer\n");
		tron_quit();
	}

	Tron->irq_timer = timer_subscribe_int();

	if (Tron->irq_timer == ERROR)
	{
		printf ("tron_boot:: Error subscribing to timer\n");
		tron_quit();
	}

	Tron->irq_mouse = mouse_subscribe_int();

	if (Tron->irq_mouse == ERROR)
	{
		printf ("tron_boot:: Error subscribing to mouse\n");
		tron_quit();
	}

	// ENABLE STREAM MODE
	for (i=0, response = 0; ((i<5) && (response != RSP_ACK))  ; i++)
	{
		mouse_write (KBC_CMD_REG, MOUSE_WRITE_CMD);
		mouse_write (IN_BUF, MOUSE_STREAM_MODE);
		mouse_read (OUT_BUF, &response);
	}
	if (response != RSP_ACK)
	{
		printf ("Timeout enabling stream mode\n");
		tron_quit();
	}
	
	//ENABLE DATA REPORTING
	for (i=0, response = 0; ((i<5) && (response != RSP_ACK))  ; i++)
	{
		mouse_write (KBC_CMD_REG, MOUSE_WRITE_CMD);
		mouse_write (IN_BUF, MOUSE_ENB);
		mouse_read (OUT_BUF, &response);
	}

	if (response != RSP_ACK)
	{
		printf ("Timeout enabling data reporting\n");
		tron_quit();
	}

	
	if(vg_init(VIDEO_MODE) == NULL)
		printf("Error initializing video mode.\n");
	Tron->video_mode = 1;
	//WAIT UNTIL USER PRESSES ENTER
	//loadBitmap....
	//initialize some variables

	mouse_create ();
	Tron->instructions = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/INSTRUCTIONS.bmp");
	Tron->scancode = 0;
	Tron->quit = 0;
	Tron->packet_counter = 0;
	Tron->first_game_tick = 0;
}


//This function reads the file and prints what it has, in this case
//it has only the actual date and current time
void read_file(FILE* fp)
{					
	int i;			
	char buff[255];

	fp = fopen("/home/lcom/lcom1617-t6g26/proj/src/example.txt", "r");

	for (i=0; i<3; i++)
	{
		fgets (buff, 255, fp);
		printf ("%s", buff);
	}

	fclose(fp);
}

//This function writes into the file the actal time and
//and current date

void write_file(FILE* fp, unsigned long* data, unsigned long* time)
{	
	int i;

	fp = fopen("/home/lcom/lcom1617-t6g26/proj/src/example.txt", "w");

	fprintf (fp,"Started playing at:\n");
	fprintf (fp,"%d/%d/%d\n", data[0], data[1], data[2]);
	fprintf (fp,"%d:%d:%d\n", time[0], time[1], time[2]);

	fclose(fp);
}

void tron_delete_state ()
{
	switch (Tron->State)
	{
		case Menu_video:
			menu_destroy();
			break;
		case Multiplayer_local:
			end_mp_local();
			break;
		case Singleplayer:
			end_singleplayer();
			break;
		case Instructions:
			break;
		default:
			/* Do nothing*/
			break;
	}
}

void tron_change_state (Game_state new_state)
{
	//delete current state
	tron_delete_state ();

	Tron->State = new_state;

	switch (Tron->State)
	{
		case Menu_video:
			start_menu ();
			break;

		case Multiplayer_local:
			Tron->first_game_tick = 1;
			start_mp_local ();
			break;
		case Singleplayer:
			Tron->first_game_tick = 1;
			start_singleplayer();
			break;
		case Instructions:
			break;
		default:
			/* Do nothing*/
			break;
	}
}

int main ()
{


	FILE* fp;
	write_file(fp, getdate(), gettime());

	int ipc_status, r;
	message msg;
	
	tron_boot ();
	tron_change_state (Menu_video);

	while (!Tron->quit)
	{
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{ /* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{
				case HARDWARE: /* hardware interrupt notification */				
					if (msg.NOTIFY_ARG & Tron->irq_timer) // timer interrupt
				 	{	
						tick();
				 	}

					if (msg.NOTIFY_ARG & Tron->irq_kbd) //keyboard interrupt 
					{
						Tron->scancode = kbd_int_handler();

						if (Tron->scancode != SPECIAL_KEY)
                		{
							keyboard_event();
						}
					}

					if (msg.NOTIFY_ARG & Tron->irq_mouse)
					{
						if (mouse_int_handler(Tron->packet, Tron->packet_counter) !=OK)
							{
								printf ("ERROR MOUSE_INT_HANDLER\n");
								return ERROR;
							}
						if (Tron->packet[0] != RSP_ACK && (Tron->packet[0] & BIT(3))) // making sure we are in sync
							Tron->packet_counter++;
						if (Tron->packet_counter == 3)
						{
							Tron->packet_counter = 0;
							mouse_event ();
						}
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

	tron_quit ();
	deleteBitmap(Tron->instructions);
	free(Tron);

	read_file(fp);



	return OK;

}

