#include <minix/syslib.h>
#include <minix/com.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <math.h>
#include "rtc.h"
#include "i8042.h"
#include "vars.h"

static unsigned long date_info[3], time_info[3];

unsigned long rtc_hex_to_dec(unsigned long hex)
{
	unsigned long dec;
	dec = (((hex) >> 4) * 10) + (hex & 0x0F);
	return dec;
}

void read_from_rtc(unsigned char address, unsigned long* info){
	sys_outb(RTC_ADDR_REG, address);
	sys_inb(RTC_DATA_REG, info);
}

unsigned long* gettime()
{
	int i;
	
	read_from_rtc(RTC_HOURS, &time_info[0]);
	read_from_rtc(RTC_MINUTES,&time_info[1]);
	read_from_rtc(RTC_SECONDS, &time_info[2]);

	for (i=0; i<3; i++)
	{
		time_info[i]=rtc_hex_to_dec(time_info[i]);
	}

	return time_info;
}

unsigned long* getdate()
{
	int i;

	read_from_rtc(RTC_DAY_OF_MONTH, &date_info[0]);
	read_from_rtc(RTC_MONTH, &date_info[1]);
	read_from_rtc(RTC_YEAR, &date_info[2]);

	for (i=0; i<3; i++)
	{
		date_info[i]=rtc_hex_to_dec(date_info[i]);
	}

	return date_info;
}
