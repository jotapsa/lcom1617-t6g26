#include <limits.h>
#include <errno.h>
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "pixmap.h"


static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static unsigned short parse_ushort (char *str, int base);
static short parse_short(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	/*synchronize with the RS*/
	sef_startup();
	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_init <mode - video mode, delay - seconds to stay in graphics mode(mode) before returning to default text mode>\"\n"
			"\t service run %s -args \"test_square <x, y - square upper left corner coordinates, size - side in pixels, color - square color>\"\n"
			"\t service run %s -args \"test_line <xi, yi - line start coordinates, xf, yf - line end coordinates, color -> line color\"\n"
			"\t service run %s -args \"test_xpm <xi, yi - xpm upper left corner coordinates, xpm- xpm image>\"\n"
			"\t service run %s -args \"test_move <xi, yi - initial xpm upper left corner coordinates, xpm- xpm image, hor- direction, delta - number of pixels to move the sprite, time - number of seconds this movement takes>\"\n"
			"\t service run %s -args \"test_controller <>\"\n",
			argv[0], argv[0], argv[0], argv[0],argv[0],argv[0]);
}


static int proc_args(int argc, char **argv)
{
	int i;
	unsigned short mode, delay, x, y, size, xf, yf, hor, time;
	unsigned long color;
	short delta;
	char** pixmap;

	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("test_init: wrong no. of arguments for test_init()\n");
			return 1;
		}
		mode = parse_ushort (argv[2], 10);
		delay = parse_ushort (argv[3],10);
		if ((mode == USHRT_MAX)||(delay==USHRT_MAX))
			return 1;

		printf("graphic::test_init(%d, %d)\n", mode, delay);
		return test_init(mode, delay);
	}
	else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {
		if (argc != 6) {
			printf("graphic: wrong no of arguments for test_square()\n");
			return 1;
		}
		x = parse_ushort (argv[2], 10);
		y = parse_ushort (argv[3], 10);
		size = parse_ushort (argv[4], 10);

		if ((x == USHRT_MAX) || (y == USHRT_MAX) || (size == USHRT_MAX))
			return 1;

		color = parse_ulong (argv[5], 10);
		if (color == ULONG_MAX)
			return 1;

		printf("graphic::test_square(%d, %d, %d, %ld)\n", x, y, size, color);
		return test_square(x, y, size, color);
	}

	else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) 
	{
		if (argc != 7) {
			printf("graphic: wrong no. of arguments for test_line()\n");
			return 1;
		}

		x = parse_ushort (argv[2], 10);
		y = parse_ushort (argv[3], 10);
		xf = parse_ushort (argv[4], 10);
		yf = parse_ushort (argv[5], 10);

		if ((x == USHRT_MAX) || (y == USHRT_MAX) || (xf == USHRT_MAX) || (yf == USHRT_MAX))
			return 1;

		color = parse_ulong (argv[6], 10);
		if (color == ULONG_MAX)
			return 1;


		printf ("graphic::test_line(%d, %d, %d, %d, %ld)\n", x, y , xf, yf, color);
		return test_line(x, y, xf, yf, color);
	}

	else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) 
	{
		if (argc != 5) {
			printf("graphic: wrong no of arguments for test_xpm()\n");
			return 1;
		}
		x = parse_ushort (argv[2], 10);
		y = parse_ushort (argv[3], 10);

		if ((x == USHRT_MAX) || (y == USHRT_MAX))
			return 1;

		if (strncmp(argv[4], "pic1", strlen("pic1")) == 0) 
		{
			pixmap = pic1;
		} 
		else if (strncmp(argv[4], "pic2", strlen("pic2")) == 0) 
		{
			pixmap = pic2;
		} 
		else if (strncmp(argv[4], "pic3", strlen("pic3")) == 0) 
		{
			pixmap = pic3;
		} 
		else if (strncmp(argv[4], "cross", strlen("cross")) == 0) 
		{
			pixmap = cross;
		} 
		else if (strncmp(argv[4], "penguin", strlen("penguin")) == 0) 
		{
			pixmap = penguin;
		} 
		else 
		{
			printf("lab5::pixmap not found\n");
			return 1;
		}

		printf("graphic::test_xpm(%d, %d, %s)\n", x, y, argv[4]);
		return test_xpm(x, y, pixmap);
	}
	else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) 
	{
		if (argc != 8) {
			printf("graphic: wrong no. of arguments for test_line()\n");
			return 1;
		}

		x = parse_ushort (argv[2], 10);
		y = parse_ushort (argv[3], 10);
		hor = parse_ushort (argv[5], 10);
		time = parse_ushort (argv[7], 10);

		if ((x == USHRT_MAX) || (y == USHRT_MAX) || (hor == USHRT_MAX) || (time == USHRT_MAX))
			return 1;

		if (strncmp(argv[4], "pic1", strlen("pic1")) == 0) 
		{
			pixmap = pic1;
		} 
		else if (strncmp(argv[4], "pic2", strlen("pic2")) == 0) 
		{
			pixmap = pic2;
		} 
		else if (strncmp(argv[4], "pic3", strlen("pic3")) == 0) 
		{
			pixmap = pic3;
		} 
		else if (strncmp(argv[4], "cross", strlen("cross")) == 0) 
		{
			pixmap = cross;
		} 
		else if (strncmp(argv[4], "penguin", strlen("penguin")) == 0) 
		{
			pixmap = penguin;
		} 
		else 
		{
			printf("lab5::pixmap not found\n");
			return 1;
		}

		delta = parse_short (argv[6], 10);

		if (delta == SHRT_MAX)
			return 1;

		printf ("graphic::test_move(%d, %d, %s, %d ,%d, %d)\n", x, y, argv[4], hor, delta, time);
		return test_move(x, y, pixmap, hor, delta, time);
	}

	else if (strncmp(argv[1], "test_controller", strlen("test_controller")) == 0) 
	{
		if (argc != 2) {
			printf("graphic: wrong no. of arguments for test_line()\n");
			return 1;
		}

		printf ("graphic::test_controller()\n", x, y);
		return test_controller();
	}
	else {
		printf("kbd: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static short parse_short(char *str, int base) 
{
	char *endptr;
	unsigned long val;

	/*Convert string to signed long*/
	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0) || (errno == ERANGE && (val < SHRT_MIN || val > SHRT_MAX))) 
	{
		perror("strtol");
		return SHRT_MAX;
	}

	if (endptr == str) {
		printf("parse_long: no digits were found in %s \n", str);
		return SHRT_MAX;
	}

	/* Successful conversion */
	return (short) val;
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

static unsigned short parse_ushort (char *str, int base)
{

	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long using strtoul and then to unsigned short */
	val = (unsigned short) strtoul(str, &endptr, base);			

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("parse_ushort: no digits were found in %s\n", str);
		return USHRT_MAX	;
	}

	/* Successful conversion */
	return val;
}
