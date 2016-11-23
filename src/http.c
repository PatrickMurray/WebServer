#include "headers.h"


void http_digest_initial_line(
	struct http_request request,
	char**              tokens,
	size_t              token_length
)
{
	return;
}

void http_digest_header_line(
	struct http_request request,
	char**              tokens,
        size_t              token_length
)
{
	return;
}


int http_valid_request_method(struct http_request request, char* method)
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


int http_valid_request_protocol_version(
	struct http_request request,
	char*               protocol
)
{
	int valid;

	valid = 0;
	
	if (strcasecmp(protocol, "HTTP") == 0)
	{
		valid = 1;
	}

	return valid;
}


int http_valid_request_protocol_version_major(
	struct http_request request,
	char*               major
)
{
	int      valid;
	long int major_val;
	char*    remainder;

	valid = 0;
	
	major_val = strtol(major, &remainder, 10);

	if (strlen(remainder) != 0)
	{
		return -1;
	}

	if (major_val == LONG_MIN || major_val == LONG_MAX)
	{
		return -1;
	}

	return valid;
}


int http_valid_request_protocol_version_minor(
	struct http_request request,
        char*               minor
)
{
	int      valid;
	long int minor_val;
	char*    remainder;

	valid = 0;

	minor_val = strtol(minor, &remainder, 10);
	
	if (strlen(remainder) != 0)
	{
		return -1;
	}

	if (minor_val == LONG_MIN || minor_val == LONG_MIN)
	{
		return -1;
	}

	return valid;
}


void http_generate_response(
	struct http_request*  http_request,
	struct http_response* http_response
)
{
	return;
}
