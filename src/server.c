#include "headers.h"

void init_server()
{
	int                 server_fd;
	struct sockaddr_in6 server_addr;
	int                 client_fd;
	struct sockaddr     client_sockaddr;
	socklen_t           client_addr_len;
	pthread_t           thread_id;
	int                 errcode;
	
	if (signal(SIGINT,  server_signal_handler) == SIG_ERR ||
	    signal(SIGTERM, server_signal_handler) == SIG_ERR)
	{
		fprintf(stderr,
			"%s: unable to set server signal handler: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	if ((server_fd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		fprintf(stderr,
			"%s: unable to open server socket: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin6_family = AF_INET6;
	server_addr.sin6_port   = htons(SERVER_PORT);
	server_addr.sin6_addr   = in6addr_any;
	
	if (bind(server_fd, (struct sockaddr*) &server_addr,
	         sizeof(server_addr)) == -1)
	{
		fprintf(stderr,
			"%s: unable to bind server socket: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, SERVER_QUEUE_CAPACITY) == -1)
	{
		fprintf(stderr,
			"%s: unable to listen on the server socket: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		memset(&client_sockaddr, 0, sizeof(client_sockaddr));
		memset(&client_addr_len, 0, sizeof(client_addr_len));

		if ((client_fd = accept(server_fd, &client_sockaddr,
		                        &client_addr_len)) == -1)
		{
			fprintf(stderr,
				"%s: unable to accept client connection: %s\n",
				getprogname(),
				strerror(errno)
			);
			exit(EXIT_FAILURE);
		}
		
		if ((errcode = pthread_create(&thread_id, NULL, handle_request, (void*) (intptr_t) client_fd)) != 0)
		{
			fprintf(stderr,
				"%s: unable to create thread: %s\n",
				getprogname(),
				strerror(errcode)
			);
			
			if (close(client_fd) == -1)
			{
				fprintf(stderr,
					"%s: unable to refuse client connection\n",
					getprogname()
				);
			}
		}
		
		if ((errcode = pthread_detach(thread_id)) != 0)
		{
			fprintf(stderr,
				"%s: unable to detach thread: %s\n",
				getprogname(),
				strerror(errcode)
			);
			exit(EXIT_FAILURE);
		}
	}
	
	pthread_exit(NULL);

	if (close(server_fd) == -1)
	{
		fprintf(stderr,
			"%s: unable to close server socket: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
}



void server_signal_handler(int signum)
{
	pthread_exit(NULL);
	exit(signum);
}
