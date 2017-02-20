#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "vars.h"
#include "timer.h"
#include "bitmap.h"
#include "mp_local.h"
#include "tron.h"

extern Program *Tron;
extern timer_struct *timer;

static unsigned long contador = 0;
static int hook_timer = 1;

int timer_subscribe_int() 
{
	/*Subscribes and enables the timer interrupts*/

	int IN_BIT = hook_timer;

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_timer) != 0 || sys_irqenable(&hook_timer) != 0)
		return ERROR;
	return BIT(IN_BIT);
}

int timer_unsubscribe_int() {

	/*Unsubscribes the timer interrupts*/

	if(sys_irqdisable(&hook_timer) != 0 || sys_irqrmpolicy(&hook_timer) != 0)
		return ERROR;
	return OK;
}

void timer_int_handler() 
{
	/*Handles the timer interruptions*/
	contador++;
}

int timer_test_int(unsigned long time)
{
	int ipc_status, r;
	int irq_timer = timer_subscribe_int ();
	message msg;

	if (irq_timer != OK)
	{
		printf ("Error subscribing to timer\n");
		return irq_timer;
	}

	time *= TIMER0_DEFAULT_FREQ;

	while(contador < time)
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
                    	timer_int_handler();
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
	
	if (timer_unsubscribe_int() != OK)
	{
		printf ("Error unsubscribing to timer\n");
		return ERROR;
	}

	return OK;
}

void timer_create ()
{
	timer = (timer_struct*) malloc (sizeof(timer_struct));
	timer->time_image[2] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/timer/3.bmp");
	timer->time_image[1] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/timer/2.bmp");
	timer->time_image[0] = loadBitmap("/home/lcom/lcom1617-t6g26/proj/images/timer/1.bmp");
	timer->contador_start =180;
	timer->contador_end = 60;

}

void timer_draw (short *buffer)
{
	unsigned short h_res, v_res;

	if (Tron->video_mode)
	{
		h_res = getHorResolution();
		v_res = getVerResolution();	
	}
	else
	{
		h_res = 1024;
		v_res = 768;
	}	

	if (timer->contador_start <= 180 && timer->contador_start>120)
	{
		drawBitmap_greenscreen(timer->time_image[2], h_res/2, 100, ALIGN_CENTER, buffer);
	}
	else if (timer->contador_start <= 120 && timer->contador_start >60)
	{
		drawBitmap_greenscreen(timer->time_image[1], h_res/2, 100, ALIGN_CENTER, buffer);
	}
	else if (timer->contador_start <= 60 && timer->contador_start >=0)
	{
		drawBitmap_greenscreen(timer->time_image[0], h_res/2, 100, ALIGN_CENTER, buffer);
	}
}

void timer_destroy ()
{
	int i;

	for (i=0; i<3; i++)
	{
		deleteBitmap(timer->time_image[i]);
	}

	free (timer);
}
