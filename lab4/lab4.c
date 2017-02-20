#include <limits.h>
#include <errno.h>
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>


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
			"\t service run %s -args \"test_packet <cnt - number of packets to process>\"\n"
			"\t service run %s -args \"test_async <idle_time - time after wich it terminates if no packets are recieved>\"\n"
			"\t service run %s -args \"test_config <> \"\n"
			"\t service run %s -args \"test_gesture <length - minimum length of the movement on the y-direction>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	int i;
	unsigned short cnt, idle_time;
	short length;

	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("test_packet: wrong no. of arguments for test_packet()\n");
			return 1;
		}
		cnt = parse_ushort (argv[2], 10);
		if (cnt == USHRT_MAX)
			return 1;

		printf("mouse::test_packet(%d)\n", cnt);
		return test_packet(cnt);
	}
	else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no of arguments for test_async()\n");
			return 1;
		}
		idle_time = parse_ushort (argv[2], 10);	
		if (idle_time == USHRT_MAX)
			return 1;

		printf("mouse::test_async(%d)\n", idle_time);
		return test_async(idle_time);
	}

	else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) 
	{
		if (argc != 2) {
			printf("mouse: wrong no. of arguments for test_config()\n");
			return 1;
		}
		printf ("mouse::test_config()\n");
		return test_config();
	}

	else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no of arguments for test_gesture()\n");
			return 1;
		}
		length = parse_short (argv[2], 10);						/* Parses string to unsigned long */
		if (length == ULONG_MAX)
			return 1;
		printf("mouse::test_gesture(%d)\n", length);
		return test_gesture(length);
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