#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"
#include "i8254.h"
#define TIMER0_DEFAULT_FREQ 60

int timer_subscribe_int(void ) 
{
	/*Subscribes and enables the timer interrupts*/

	int IN_BIT = hook_timer;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_timer) != 0 || sys_irqenable(&hook_timer) != 0)
		return -1;

	return BIT(IN_BIT);
}

int timer_unsubscribe_int() {

	/*Unsubscribes the timer interrupts*/

	if(sys_irqdisable(&hook_timer) != 0 || sys_irqrmpolicy(&hook_timer) != 0)
		return -1;

	return 0;
}

void timer_int_handler(int *seconds) 
{
	/*Handles the timer interruptions*/
	contador++;
	if (contador % TIMER0_DEFAULT_FREQ == 0)
	{
		*seconds= *seconds +1;
	}
}