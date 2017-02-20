#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#define TIMER0_DEFAULT_FREQ 60

static int contador = 0;
static int hook_id = 4;
static int seconds = 0;

int timer_set_square(unsigned long timer, unsigned long freq) 
{
 	//Validates the timer
 	if (!((timer>=0) && (timer<3)))
 		return -1;

 	unsigned char conf, timer_selection;
 	unsigned int TIMER_DIV = TIMER_FREQ / freq;

 	timer_get_conf (timer, &conf); //Gets the present configuration of the timer

 	switch (timer)
 	{
	case 0:
		timer_selection = TIMER_SEL0;
		break;
	case 1:
		timer_selection = TIMER_SEL0;
		break;
	case 2:
		timer_selection = TIMER_SEL2;
		break;
 	}


 	if (sys_outb(TIMER_CTRL, (timer_selection | TIMER_LSB_MSB | TIMER_SQR_WAVE | (conf & BIT(0)))) !=0) 
 		return -1;
 	if (sys_outb((TIMER_0 + timer), (TIMER_DIV & 0xff))!=0)  //Writes in the least significant bite of the counter
 		return -1;
 	if (sys_outb((TIMER_0 + timer), ((TIMER_DIV >> 8) & 0Xff)) !=0) //Writes in the most significative bite of the counter
 		return -1;

	return 0;
}


int timer_subscribe_int(void ) 
{
	/*Subscribes and enables the timer interrupts*/

	int IN_BIT = hook_id;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0 || sys_irqenable(&hook_id) != 0)
		return -1;

	return IN_BIT;
}

int timer_unsubscribe_int() {

	/*Unsubscribes the timer interrupts*/

	if(sys_irqdisable(&hook_id) != 0 || sys_irqrmpolicy(&hook_id) != 0)
		return -1;

	return 0;
}

void timer_int_handler() 
{
	/*Handles the timer interruptions*/

	contador++;
	if (contador % TIMER0_DEFAULT_FREQ == 0){
		seconds++;
		printf ("Passaram %d segundo(s)\n", seconds);
	}
}

int timer_get_conf(unsigned long timer, unsigned char *st)
{
	/*Reads the timer configuration via read-back command*/

	if (sys_outb(TIMER_CTRL, (TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ | TIMER_RB_CMD))!= 0)
		return -1;
	if (sys_inb((TIMER_0 + timer),(unsigned long*) st)!=0)
		return -1;
	return 0;
	
}

int timer_display_conf(unsigned char conf)
{
	printf("OUT PIN = %s\n", (conf & BIT(7)) ? "TRUE" : "FALSE");       // (OUT PIN)

	printf("NULL COUNT = %s\n", (conf & BIT(6)) ? "TRUE" : "FALSE");    // (NULL COUNT)

	switch (conf & (BIT(4) | BIT(5))) 						            // (Type of Access)
	{	
		case TIMER_LSB:
			printf("Type of Access: LSB\n");
			break;
		case TIMER_MSB:											
			printf("Type of Access: MSB\n");
			break;
		case TIMER_LSB_MSB:
			printf("Type of Access: LSB followed by MSB\n");
			break;
		default:
			printf ("Type of access: invalid\n");
			return -1;
	} 

	switch(conf & (BIT(3) | BIT(2) | BIT(1)))				            // (Operating mode)
	{
		case 0:
			printf ("MODE 0: INTERRUPT ON TERMINAL COUNT\n");
			break;
		case BIT(1):
			printf ("MODE 1: HARDWARE RETRIGGERABLE ONE-SHOT\n");
			break;
		case TIMER_RATE_GEN:
			printf ("MODE 2: RATE GENERATOR\n");
			break;
		case TIMER_SQR_WAVE:
			printf ("MODE 3: SQUARE WAVE MODE\n");
			break;
		case BIT (3):
			printf ("MODE 4: SOFTWARE TRIGGERED STROBE\n");
			break;
		case (BIT(3) | BIT(1)):
			printf ("MODE 5: HARDWARE TRIGGERED STROBE (RETRIGGERABLE)\n");
			break;
	}

	printf("Counting mode: %s\n", (conf & BIT(0)) ? "BCD" : "Binary");  // (Counting mode)

	return 0;
}

int timer_test_square(unsigned long freq) 
{
	return timer_set_square (0, freq); 
}

int timer_test_int(unsigned long time)
{
	int ipc_status, r;
	int irq_set = timer_subscribe_int ();
	message msg;

	// timer_subscribe_int() returns a negative value if unsuccessful
	if (irq_set < 0)
			return irq_set;

	while(contador < (time * TIMER0_DEFAULT_FREQ)) //Used 60 because its the default frequency
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
                	if (msg.NOTIFY_ARG & BIT(irq_set)) /* subscribed interrupt */
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
	
	return timer_unsubscribe_int();
}

int timer_test_config(unsigned long timer)
{
	if ((timer>=0) && (timer<3))
	{
		unsigned char conf;
		timer_get_conf (timer, &conf);
		timer_display_conf(conf);
		return 0;
	}
	else 				//The requested timer does not exists
		return -1;
}
