#include "headers.h"


extern int SERVER_PORT;


int
main(int argc, char** argv)
{
	setprogname(argv[0]);
	
	arguments_parse(argc, argv);
	server_init();
	
	return EXIT_SUCCESS;
}
