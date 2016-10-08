#include "headers.h"


int http_valid_method(char* method)
{
	int valid;

	valid = 0;

	if (strcasecmp(method, "OPTIONS") == 0 ||
	    strcasecmp(method, "GET"    ) == 0 ||
	    strcasecmp(method, "HEAD"   ) == 0 ||
	    strcasecmp(method, "POST"   ) == 0 ||
	    strcasecmp(method, "PUT"    ) == 0 ||
	    strcasecmp(method, "DELETE" ) == 0 ||
	    strcasecmp(method, "TRACE"  ) == 0 ||
	    strcasecmp(method, "CONNECT") == 0)
	{
		valid = 1;
	}

	return valid;
}


int http_valid_version_protocol(char* protocol)
{
	int valid;

	valid = 0;
	
	if (strcasecmp(protocol, "HTTP") == 0)
	{
		valid = 1;
	}

	return valid;
}


int http_valid_version_major(char* major)
{
	int valid;
	int major;
	
	valid = 0;

	return valid;
}


int http_valid_version_minor(char* minor)
{
	int valid;
	int minor;

	valid = 0;

	return valid;
}
