#include "test4.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>


int test_packet(unsigned short cnt)
{
	int ipc_status, r;
	message msg;
	int irq_mouse = mouse_subscribe_int();
	unsigned char packet[3], response=0;
	int i, counter=0;

	if (irq_mouse == ERROR)
	{
		printf ("kbd_subscribe_int returned ERROR value\n");
		return irq_mouse;
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
		return ERROR;
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
		return ERROR;
	}

	for (i=0; (i<cnt); i++)
	{
		counter=0;
		while (counter<3)
		{
			r= driver_receive(ANY, &msg, &ipc_status);
			if (r != OK)
			{
	       		printf("test_packet: driver_receive failed with: %d", r);
		   		continue;
			}
			if (is_ipc_notify(ipc_status))
			{
				switch (_ENDPOINT_P(msg.m_source))
				{
					case HARDWARE:
						if (msg.NOTIFY_ARG & irq_mouse)
						{
							if (mouse_int_handler(packet, counter) !=OK)
							{
								printf ("ERROR MOUSE_INT_HANDLER\n");
								return ERROR;
							}
							if (packet[0] != RSP_ACK && (packet[0] & BIT(3))) // making sure we are in sync
								counter++;
						}
						break;
					default:
						break;
				}
			}
			else
			{	
			}
		}
		mouse_print(packet);
	}
	//DISABLE DATA REPORTING
	for (i=0, response = 0; ((i<5) && (response != RSP_ACK))  ; i++)
	{
		mouse_write (KBC_CMD_REG, MOUSE_WRITE_CMD);
		mouse_write (IN_BUF, MOUSE_DISB);
		mouse_read (OUT_BUF, &response);
	}

	if (mouse_unsubscribe_int() != OK)
		return ERROR;
	return empty_OUT_BUF();
}	
	
int test_async(unsigned short idle_time)
{
	int ipc_status, r;
	int irq_timer = timer_subscribe_int ();
	int irq_mouse = mouse_subscribe_int();
	message msg;
	unsigned char response, packet[3];
	int i, counter = 0;

	if (irq_timer == ERROR)
	{
		printf ("timer_subscribe_int returned ERROR value\n");
		return irq_timer;
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
		return ERROR;
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
		return ERROR;
	}

	while(seconds < idle_time) 
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
                    	timer_int_handler(&seconds);
        			}
                    if (msg.NOTIFY_ARG & irq_mouse)
                    {
                    	seconds=0;
                    	if (mouse_int_handler(packet, counter) !=OK)
						{
							printf ("ERROR Mouse_int_handler()\n");
							return ERROR;
						}

						if (packet[0] != RSP_ACK && (packet[0] & BIT(3))) // making sure we are in sync
							counter++;

						if(counter==3)
						{
							counter=0;
							mouse_print(packet);
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
	printf ("test_async() timed out\n");
	
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
		return ERROR;
	}

	if (timer_unsubscribe_int() != OK)
		return ERROR;

	if (mouse_unsubscribe_int() != OK)
		return ERROR;

	return empty_OUT_BUF();
}
	
int test_config() 
{
	unsigned char status = 0;
	int i, irq_mouse = mouse_subscribe_int();
	unsigned char packet[3], tmp, response;

	if (irq_mouse == ERROR)
	{
		printf ("mouse_subscribe_int returned ERROR value\n");
		return ERROR;
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
		return ERROR;
	}

	// REQUEST MOUSE STATUS
	for (i=0, response = 0; ((i<5) && (response != RSP_ACK))  ; i++)
	{
		mouse_write (KBC_CMD_REG, MOUSE_WRITE_CMD);
		mouse_write (IN_BUF, MOUSE_STATUS_REQ);
		mouse_read (OUT_BUF, &response);
	}

	if (response != RSP_ACK)
	{
		printf ("Timeout requesting mouse status\n");
		return ERROR;
	}
	

	for (i=0, response = 0; i<3; i++)
	{
		if (mouse_read(OUT_BUF, &tmp) !=OK)
		{
			printf ("ERROR reading mouse status\n");
			return ERROR;
		}
		packet[i] = tmp;
	}

	//Remote mode
	printf ("%s\n", packet[0] & BIT(6) ? "Remote Mode" : "Stream Mode");
	//Data Reporting mode
	printf ("Data reporting: %s\n", packet[0] & BIT(5) ? "Enabled" : "Disabled");
	//Scaling mode
	printf ("Scaling: %s\n", packet[0] & BIT(4) ? "2:1" : "1:1");
	//Left Button
	printf ("Left Button: %s\n", packet[0] & BIT (2) ? "Pressed": "Released");
	//Middle Button
	printf ("Middle Button: %s\n", packet[0] & BIT (1) ? "Pressed" : "Released");
	//Right Button
	printf ("Right Button: %s\n", packet[0] & BIT (0) ? "Pressed" : "Released");

	//Resolution
	printf ("Resolution : %d\n", packet[1]);

	//Sample rate
	printf("Sample rate : %d\n", packet[2]);

	if (mouse_unsubscribe_int() != OK)
		return ERROR;
	return empty_OUT_BUF ();
}	
	
int test_gesture(short length) 
{
	int ipc_status, r;
	message msg;
	int irq_mouse = mouse_subscribe_int();
	unsigned char packet[3], response=0;
	int i, counter=0;
	static state_t st = init;

	if (irq_mouse == ERROR)
	{
		printf ("kbd_subscribe_int returned ERROR value\n");
		return irq_mouse;
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
		return ERROR;
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
		return ERROR;
	}

	while(st != comp) 
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
                    if (msg.NOTIFY_ARG & irq_mouse)
                    {
                    	if (mouse_int_handler(packet, counter) !=OK)
						{
							printf ("ERROR Mouse_int_handler()\n");
							return ERROR;
						}

						if (packet[0] != RSP_ACK && (packet[0] & BIT(3))) // making sure we are in sync
							counter++;

						if(counter==3)
						{
							counter=0;
							mouse_print(packet);
							moore_machine (packet, &st, length);
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

	printf ("test_gesture :: Drew line\n");

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
		return ERROR;
	}

	if (mouse_unsubscribe_int() != OK)
		return ERROR;
	return empty_OUT_BUF();
}	
