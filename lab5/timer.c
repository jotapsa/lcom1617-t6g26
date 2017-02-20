#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"
#include "i8042.h"
#include "timer.h"

static int contador = 0;
static int hook_timer = 4;
static int seconds = 0;

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
	if (contador % TIMER0_DEFAULT_FREQ == 0)
	{
		seconds++;
	}
}

int timer_test_int(unsigned long time)
{
	int ipc_status, r;
	int irq_timer = timer_subscribe_int ();
	message msg;

	// timer_subscribe_int() returns a negative value if unsuccessful
	if (irq_timer == ERROR)
	{
		printf ("timer_subscribe_int returned ERROR value\n");
		return ERROR;
	}

	while(seconds < time) 
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
		printf("Error unsubscribing timer\n");
		return ERROR;
	}
	return OK;
}
