#include "headers.h"


int main(int argc, char** argv)
{
	setprogname(argv[0]);
	
	parse_arguments(argc, argv);
	init_server();
	
	return EXIT_SUCCESS;
}
