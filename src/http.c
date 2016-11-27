#include "headers.h"


void
http_init_request(struct http_request* request)
{
	request->method        = NULL;
	request->resource      = NULL;
	request->protocol      = NULL;
	request->version_major = -1;
	request->version_minor = -1;
	return;
}


void
http_init_response(struct http_response* response)
{
	response->status_code = -1;
	return;
}


void
http_free_request(struct http_request* request)
{
	free(request->method);
	free(request->resource);
	free(request->protocol);
	return;
}


void
http_free_response(struct http_response* response)
{
	return;
}


int
http_digest_initial_line(struct http_request* request,
                         struct http_response* response, char** tokens,
                         size_t token_length)
{
	int    status;
	size_t idx;
	char** protocol_tokens;
	size_t protocol_tokens_length;
	char** protocol_version_tokens;
	size_t protocol_version_tokens_length;
	long   ret;
	
	status = 0;
	
	for (idx = 0; idx < token_length; idx++)
	{
		if (idx == 0)
		{
			if ((request->method = strdup(tokens[idx])) == NULL)
			{
				/* Unable to allocate memory */
				status = -1;

				/* 500 Internal Server Error */
			}
		}
		else if (idx == 1)
		{
			if ((request->resource = strdup(tokens[idx])) == NULL)
			{
				/* Unable to allocate memory */
				status = -1;
				
				/* 500 Internal Server Error */
			}
		}
		else if (idx == 2)
		{
			/* TODO protocol + major + minor */
			protocol_tokens_length = 0;
			protocol_tokens = tokenize(tokens[idx], "/",
				&protocol_tokens_length
			);

			if (protocol_tokens_length != 2)
			{
				status = -1;
				/* 400 Bad Request */
			}

			if ((request->protocol = strdup(tokens[idx])) == NULL)
			{
				/* Unable to allocate memory */
				status = -1;
				/* 500 Internal Server Error */
			}

			protocol_version_tokens_length = 0;
			protocol_version_tokens = tokenize(protocol_tokens[1],
				".", &protocol_version_tokens_length
			);
			
			if (protocol_version_tokens_length != 2)
			{
				status = -1;
				/* 400 Bad Request */
			}

			printf("%s\n", protocol_version_tokens[0]);
			ret = 0;
			
			if (ret == 0)
			{
				/* Major 0.X not supported */
				/* TODO */
			}

			free_tokens(protocol_version_tokens,
				protocol_version_tokens_length
			);
			free_tokens(protocol_tokens, protocol_tokens_length);
		}
		else
		{
			/* If there're more than 3 tokens */
			status = -1;
			/* 400 Bad Request */
		}
	}
	
	return status;
}


int
http_digest_header_line(struct http_request* request,
                        struct http_response* response, char** tokens,
                        size_t token_length)
{
	int status;

	status = 0;

	/* Header Support Not Implemented */
	
	return status;
}


void
http_generate_response(struct http_request* http_request,
                       struct http_response* http_response)
{
	if (http_request->method != NULL && http_request->resource != NULL)
	{
		printf("%s -> %s\n",
			http_request->method,
			http_request->resource
		);
	}
	return;
}


int
http_valid_request_method(struct http_request* request, char* method)
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


int
http_valid_request_protocol_version(struct http_request* request,
                                    char* protocol)
{
	int valid;

	valid = 0;
	
	/* TODO : Update, HTTP/1.1 protocol is case sensitive per RFC */
	if (strcasecmp(protocol, "HTTP") == 0)
	{
		valid = 1;
	}

	return valid;
}


int
http_valid_request_protocol_version_major(struct http_request* request,
                                          char* major)
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


int
http_valid_request_protocol_version_minor(struct http_request* request,
                                          char* minor)
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
