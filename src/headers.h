#ifndef _HEADER_H_
#define _HEADER_H_


#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <getopt.h>
#include <limits.h>
#include <strings.h>

#ifndef __NetBSD__
#include <bsd/stdlib.h>
#endif


/* When set to 1, verbose output CAN NOT be disabled at run-time */
#ifndef DEFAULT_VERBOSE_MODE
#define DEFAULT_VERBOSE_MODE 0
#endif


#ifndef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT 8080
#endif

#ifndef SERVER_PORT_MIN
#define SERVER_PORT_MIN 1
#endif

#ifndef SERVER_PORT_MAX
#define SERVER_PORT_MAX 65535
#endif


#ifndef SERVER_QUEUE_CAPACITY
#define SERVER_QUEUE_CAPACITY 1024
#endif

#ifndef HEADER_BUFFER_LENGTH
#define HEADER_BUFFER_LENGTH 4096
#endif


#ifndef HTTP_VERSION_MAJOR
#define HTTP_VERSION_MAJOR 1
#endif


#ifndef HTTP_VERSION_MINOR
#define HTTP_VERSION_MINOR 1
#endif


int SERVER_PORT;
int VERBOSE_MODE;


struct http_request
{
	char*               method;
	char*               resource;
	
	char*               protocol;
	int                 version_major;
	int                 version_minor;

	struct http_header* headers;
	size_t              headers_length;
};


struct http_header
{
	char*  entity;
	char** values;
	size_t values_length;
};


struct http_response
{
	int status_code;
};


/* main.c */
int main(int, char**);


/* arguments.c */
void arguments_parse(int, char**);
void arguments_usage(FILE*);


/* server.c */
void  server_init();
void  server_signal_handler(int);


/* request.c */
void*  request_handler(void*);
char*  request_getline(int, char*, size_t);


/* token.c */
char** tokenize(char*, char*, size_t*);
void   free_tokens(char**, size_t);


/* http.c */
void http_init_request(struct http_request*);
void http_init_response(struct http_response*);
void http_free_request(struct http_request*);
void http_free_response(struct http_response*);

int http_digest_initial_line(struct http_request*, struct http_response*,
                             char**, size_t);
int http_digest_header_line(struct http_request*, struct http_response*,
                            char**, size_t);

int http_valid_request_method(struct http_request*, char*);
int http_valid_request_protocol_version(struct http_request*, char*);
int http_valid_request_protocol_version_major(struct http_request*, char*);
int http_valid_request_protocol_version_minor(struct http_request*, char*);

void http_generate_response(struct http_request*, struct http_response*);
void http_send_response(struct http_response*, int);

#endif
