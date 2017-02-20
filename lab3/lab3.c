#include "test3.h"
#include "i8042.h"
#include "i8254.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>


static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	/*synchronize with the RS*/
	sef_startup();
    /* Enable IO-sensitive operations for ourselves */
    sys_enable_iop(SELF);
	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"scan <0 - Use IH written in C !0 - Use IH written in Assembly>\"\n"
			"\t service run %s -args \"leds <array - toggles led status use values 0<=led<=2 >\"\n"
			"\t service run %s -args \"scan <decimal no. - time>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	int i;
	unsigned short mode, *toggle, time, num_elem;

	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("scan: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		mode = (unsigned short) parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (mode == ULONG_MAX)
			return 1;

		printf("kbd::kbd_test_scan(%lu)\n", mode);
		return kbd_test_scan(mode);
	}
	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) 
	{
		if (argc < 3) {
			printf("kbd: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}
		num_elem = argc - 2;
		toggle = (unsigned short*) malloc((sizeof(unsigned short))*num_elem);
		if (toggle == NULL)
		{
			printf ("Error alocating memory");
			return 1;
		}
		for (i=0 ; i< num_elem; i++)
		{
			if (parse_ulong(argv[2+i],10) == ULONG_MAX)
				return 1;
			toggle[i] = (unsigned short) parse_ulong(argv[2+i],10);
		}

		printf("kbd::kbd_test_leds(%d, '", num_elem);
		for (i=0; i<num_elem; i++)
			printf ("%d ", toggle[i]);
		printf("')\n");
		return kbd_test_leds (num_elem, toggle);
	}
	else if (strncmp(argv[1], "timed", strlen("timed")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no of arguments for kbd_test_timed()\n");
			return 1;
		}
		time = (unsigned short) parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("kbd::kbd_test_timed_scan(%lu)\n", time);
		return kbd_test_timed_scan(time);
	}
	else {
		printf("kbd: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);			

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("timer: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
