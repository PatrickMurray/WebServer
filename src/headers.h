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


#ifndef SERVER_PORT
#define SERVER_PORT 34543
#endif

#ifndef SERVER_QUEUE_CAPACITY
#define SERVER_QUEUE_CAPACITY 128
#endif

#ifndef HEADER_BUFFER_LENGTH
#define HEADER_BUFFER_LENGTH 1024
#endif


/* main.c */
int main(int, char**);

/* arguments.c */
void parse_arguments(int, char**);


/* server.c */
void init_server();
void server_signal_handler(int);

/* request.c */
void* handle_request(void*);
char* request_getline(int, char*, size_t);
