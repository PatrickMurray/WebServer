#include <stdio.h>
#include <stdlib.h>

#ifndef __NetBSD__
#include <bsd/stdlib.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <getopt.h>
#include <limits.h>
#include <strings.h>


#ifndef DEFAULT_SERVER_PORT
#define DEFAULT_SERVER_PORT 8080
#endif

#ifndef SERVER_PORT_MIN
#define SERVER_PORT_MIN 1
#endif

#ifndef SERVER_PORT_MAX
#define SERVER_PORT_MAX 65535
#endif


#ifndef DEFAULT_VERBOSE_MODE
#define DEFAULT_VERBOSE_MODE 0
#endif


#ifndef SERVER_QUEUE_CAPACITY
#define SERVER_QUEUE_CAPACITY 128
#endif

#ifndef HEADER_BUFFER_LENGTH
#define HEADER_BUFFER_LENGTH 1024
#endif


int SERVER_PORT;
int VERBOSE_MODE;


/* main.c */
int main(int, char**);

/* arguments.c */
void arguments_parse(int, char**);
void arguments_usage(FILE*);


/* server.c */
void  server_init();
void  server_signal_handler(int);
char* getsockaddr(int);

/* request.c */
void*  request_handler(void*);
char*  request_getline(int, char*, size_t);

/* token.c */
char** tokenize(char*, char*, size_t*);
void   free_tokens(char**, size_t);

/* http.c */
int http_valid_method(char*);
int http_valid_version_protocol(char*);
int http_valid_version_major(char*);
int http_valid_version_minor(char*);
