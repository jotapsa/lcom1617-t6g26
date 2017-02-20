#include "test5.h"

static int contador = 0;

void *test_init(unsigned short mode, unsigned short delay) 
{
	void* result;
	vbe_mode_info_t *vmi_p = malloc(sizeof(vbe_mode_info_t));
	
	if (vg_init(mode) == NULL)
	{
		printf ("test_init(): Couldn't switch to video mode 0x%x\n ",mode);
		return NULL;
	}

	timer_test_int((unsigned long)delay);

	if(vbe_get_mode_info(MODE_1024X768_8BPP, vmi_p) != OK)
	{
		vg_exit ();
		printf ("Error getting mode info\n");
		return NULL;
	}

	vg_exit();
	vbe_print(vmi_p);
	free (vmi_p);
	return result;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) 
{
	int i, j;
	vbe_mode_info_t *vmi_p = malloc(sizeof(vbe_mode_info_t));
	char *video_mem = vg_init(MODE_1024X768_8BPP);

	if (video_mem == NULL)
	{
		printf ("test_square(): Couldn't switch to video mode 0x%x\n ",MODE_1024X768_8BPP);
		return ERROR;
	}

	for (j=0; j<size; j++)
	{
		for (i=0; i<size; i++)
		{
			if (vg_draw_pixel (x+i, y+j, color, video_mem) != OK) /*using the vram adress we write directly in the screen  we could use a buffer, but there is no need right now*/
			{
				printf ("\ttest_square(): vg_draw_pixel() failed\n");
			}
		}
	}

	if(vbe_get_mode_info(MODE_1024X768_8BPP, vmi_p) != OK)
	{
		vg_exit ();
		printf ("Error getting mode info\n");
		return ERROR;
	}

	kbd_test_scan();
	vg_exit();
	vbe_print(vmi_p);
	free(vmi_p);

	return OK;
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) 
{
	int i, dx, dy , k;
	float x = xi, y = yi, mx, my;

	vbe_mode_info_t *vmi_p = malloc(sizeof(vbe_mode_info_t));
	char *video_mem = vg_init(MODE_1024X768_8BPP);

	if (video_mem == NULL)
	{
		printf ("test_line(): Couldn't switch to video mode 0x%x\n ",MODE_1024X768_8BPP);
		return ERROR;
	}

	dx = xf - xi;
	dy = yf - yi;

	if (abs(dx)>=abs(dy))
		k=abs(dx);
	else
		k=abs(dy);

	mx= dx / (float) k;
	my= dy / (float) k;

	vg_draw_pixel(xi, yi, color, video_mem);

	for (i=0; i<k; i++)
	{
		x = x + mx;
		y = y + my;
		if (vg_draw_pixel ((unsigned int) x,(unsigned int) y, color, video_mem) != OK) 
		{
			printf ("\ttest_line(): vg_draw_pixel() failed\n");
		}
	}

	if(vbe_get_mode_info(MODE_1024X768_8BPP, vmi_p) != OK)
	{
		vg_exit ();
		printf ("Error getting mode info\n");
		return ERROR;
	}

	kbd_test_scan();
	vg_exit();
	vbe_print(vmi_p);
	free(vmi_p);

	return OK;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) 
{
	int width, height, x, y, i=0;

	vbe_mode_info_t *vmi_p = malloc(sizeof(vbe_mode_info_t));

	Sprite *sp;

	if (vg_init (MODE_1024X768_8BPP) == NULL)
		return ERROR;

	sp = create_sprite (xpm);

	sp->x=xi;
	sp->y=yi;

	draw_sprite (sp);

	destroy_sprite(sp);

	if(vbe_get_mode_info(MODE_1024X768_8BPP, vmi_p) != OK)
	{
		vg_exit ();
		printf ("Error getting mode info\n");
		return ERROR;
	}


	kbd_test_scan ();
	vg_exit();
	vbe_print(vmi_p);
	free(vmi_p);

	return OK;
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) 
{
	/* IH Variables */
	int ipc_status, r;
	int irq_timer = timer_subscribe_int ();
	int irq_kbd = kbd_subscribe_int ();
	unsigned short scancode = 0;
	message msg;
	contador =0;

	/* Movement variables */
	char *map;
	float m, x, y, i, j;
	int width, height, k=0;

	if (irq_timer == ERROR)
	{
		printf ("timer_subscribe_int returned ERROR value\n");
		return ERROR;
	}

	time = time * TIMER0_DEFAULT_FREQ; // conversion from seconds to "ticks" or frames

	if (irq_kbd == ERROR)
	{
		printf ("kbd_subscribe_int returned ERROR value\n");
	 	return irq_kbd;
	}

	vbe_mode_info_t *vmi_p = malloc(sizeof(vbe_mode_info_t));

	if (vg_init(MODE_1024X768_8BPP) == NULL)
	{
	 	printf ("Error initializing mode 0x105\n");
	 	return ERROR;
	}	

	/*Sprite time*/
	Sprite *sp;

	sp = create_sprite (xpm);
	sp->x=xi;
	sp->y=yi;

	/*How much does the sprite move every frame? / whats the sprite speed*/

	m = (delta/(float)time);

	draw_sprite (sp);

	int exit = 0;
	while (exit == 0)
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
                	if (msg.NOTIFY_ARG & irq_timer) /* subscribed interrupt */
        			{
        				contador++;

        				//erase the last sprite position from screen
        				erase_sprite (sp);

         				//update sprite position

         				if (hor)
         					sp->x+=m;
         				else
        					sp->y+=m;

 	      				draw_sprite(sp);

                    }
					else if (msg.NOTIFY_ARG & irq_kbd) /* Keyboard interruption*/
                	{
                		if (scancode != SPECIAL_KEY)
							scancode = 0;
						else
							scancode = scancode << 8;

						scancode = kbd_int_handler_c();

						if (scancode == SPECIAL_KEY)
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

    	// Should we exit ?
    	if ((contador>=time) || (scancode == KEY_ESC_BREAK))
    		exit = 1;
	}

	destroy_sprite(sp);

	if(vbe_get_mode_info(MODE_1024X768_8BPP, vmi_p) != OK)
	{
		vg_exit ();
		printf ("Error getting mode info\n");
		return ERROR;
	}

	vg_exit();
	
	if (timer_unsubscribe_int() != OK)
	{
		printf("Error unsubscribing timer\n");
		return ERROR;
	}

	if (kbd_unsubscribe_int() != OK)
	{
		printf("Error unsubscribing kbd\n");
		return ERROR;
	}
	vbe_print(vmi_p);
	free(vmi_p);

	return OK;
}					

int test_controller() {
	vbe_controller_info_t *vbe_info = malloc(sizeof(vbe_controller_info_t));

	void* info = (void*)vbe_get_controller_info(vbe_info);
	
	if (info == NULL){
		printf("Erro");
		return ERROR;
	}
	
	printf("\t\t*****VBE INFORMATION*****\n\n");

	printf("     Capabilities:\n\n");

	printf("DAC:\t\t");
	if(vbe_info->Capabilities[0] & BIT(0))
		printf("DAC width is switchable to 8 bits per primary color.\n\n");
	else
		printf("DAC is fixed width, with 6 bits per primary colors.\n\n");

	printf("Controller:\t");
	if(vbe_info->Capabilities[1] & BIT(1))
		printf("Controller is VGA compatible.\n\n");
	else
		printf("Controller is not VGA compatible.\n\n");

	printf("RAMDAC:\t\t");
	if(vbe_info->Capabilities[2] & BIT(2))
		printf("Normal RAMDAC operation.\n\n");
	else
		printf("Large Blocks to RAMDAC.\n\n");

	printf("   Video Modes Supported:\n");

	//Gets the address
	short *phys_address = info + (((int)(vbe_info->VideoModePtr) >> 12)& 0xF0000) + ((int)vbe_info->VideoModePtr & 0x0FFFF);

	while (*phys_address != -1)
	{
		printf("0x%x\t", *phys_address);
		phys_address++;
	}

	printf("\n\nTotal Memory:  %dKB. \n", vbe_info->TotalMemory*64);

	free(vbe_info);

	return OK;
}					
	
