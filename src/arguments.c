#include "headers.h"


extern int SERVER_PORT;
extern int VERBOSE_MODE;


void
arguments_parse(int argc, char** argv)
{
	int      flag;
	char*    optstring;
	int      long_index;
	long int ret;
	char*    endptr;

	struct option long_options[] = {
		{"help",    no_argument,       0, 'h'},
		{"verbose", no_argument,       0, 'v'},
		{"port",    required_argument, 0, 'p'},
		{"root",    required_argument, 0, 'r'},
		{"log",     required_argument, 0, 'l'},
		{0,         0,                 0, 0  }
	};
	
	optstring  = "+hvp:r:l:";
	long_index = 0;
	
	SERVER_PORT  = DEFAULT_SERVER_PORT;
	VERBOSE_MODE = DEFAULT_VERBOSE_MODE;
	
	/* Disable getopt() default error message */
	opterr = 0;
	while ((flag = getopt_long(argc, argv, optstring, long_options, &long_index)) != -1) {
		switch (flag) {
			case 'h':
				arguments_usage(stdout);
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				VERBOSE_MODE = 1;
				break;
			case 'p':
				ret = strtol(optarg, &endptr, 10);
				
				if (strlen(endptr) != 0)
				{
					fprintf(stderr,
						"%s: invalid server port: unrecognized characters '%s'\n",
						getprogname(),
						endptr
					);
					exit(EXIT_FAILURE);
				}
				
				if (ret == LONG_MIN || ret == LONG_MAX)
				{
					fprintf(stderr,
						"%s: invalid server port: %s\n",
						getprogname(),
						strerror(errno)
					);
					exit(EXIT_FAILURE);
				}
				
				if (ret < SERVER_PORT_MIN || SERVER_PORT_MAX < ret)
				{
					fprintf(stderr,
						"%s: server port must be between %i and %i\n",
						getprogname(),
						SERVER_PORT_MIN,
						SERVER_PORT_MAX
					);
					exit(EXIT_FAILURE);
				}
				SERVER_PORT = (int) ret;
				break;
			case 'r':
				/* root = 1; */
				break;
			case 'l':
				/* TODO */
				break;
			case ':':
				/* TODO */
				break;
			case '?':
				fprintf(stderr,
					"%s: unknown option '-%c'\n",
					getprogname(),
					optopt
				);
			default:
				arguments_usage(stderr);
				exit(EXIT_FAILURE);
				break;
		}
	}
	return;
}


void
arguments_usage(FILE* stream)
{
	fprintf(stream, "Usage: %s [OPTION]...\n", getprogname());
	fprintf(stream, "A simple HTTP/1.1 web server.\n");
	fprintf(stream, "\n");
	fprintf(stream, "  -h, --help            displays this help message and exits\n");
	fprintf(stream, "  -v, --verbose         enables verbose output\n");
	fprintf(stream, "  -p, --port=NUM        binds the server to the provided port number within the\n");
	fprintf(stream, "                        allowable range (%i, %i)\n", SERVER_PORT_MIN, SERVER_PORT_MAX);
	fprintf(stream, "  -r, --root=DIRECTORY  specifys the server's root directory, by default if no\n");
	fprintf(stream, "                        root directory is provided, the current working\n");
	fprintf(stream, "                        directory will be used\n");
	fprintf(stream, "  -l, --log=FILE        appends access log information to the provided log file\n");
	fprintf(stream, "\n");
	fprintf(stream, "BSD 3-Clause License (Revised)\n");
	fprintf(stream, "Copyright (c) 2016 Patrick Murray, All rights reserved.\n");
	return;
}
