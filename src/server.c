#include "headers.h"


extern int SERVER_PORT;
extern int VERBOSE_MODE;


void server_init()
{
	int                     server_fd;
	struct sockaddr_in6     server_addr;
	int                     client_fd;
	struct sockaddr         client_sockaddr;
	socklen_t               client_addr_len;
	pthread_t               thread_id;
	int                     errcode;
	int                     reuseaddr;
	int                     reuseport;

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
	
	if (VERBOSE_MODE)
	{
		printf(":: Opening socket connection\n");
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
	
	if (VERBOSE_MODE)
	{
		printf(":: Setting socket options\n");
	}

	reuseaddr = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1) {
		fprintf(stderr, "%s: unable to set socket options: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	reuseport = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuseport, sizeof(reuseport)) == -1) {
		fprintf(stderr, "%s: unable to set socket options: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	if (VERBOSE_MODE)
	{
		printf(":: Binding socket\n");
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
	
	if (VERBOSE_MODE)
	{
		printf(":: Listening for socket connections\n");
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
	
	if (VERBOSE_MODE)
	{
		printf(":: Waiting for connections\n");
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
		
		if (VERBOSE_MODE) 
		{
			printf(":: Recieved connection\n");
		}
		
		if ((errcode = pthread_create(&thread_id, NULL, request_handler, (void*) (intptr_t) client_fd)) != 0)
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


/*char* getsockaddr(int socket)
{
	struct sockaddr_storage peer_sockaddr;
	socklen_t               peer_addr_len;
	uint16_t                netshort;
	uint16_t                port;
	struct sockaddr_in      ipv4;
	struct sockaddr_in6     ipv6;
	
	int       af;
	void*     src;
	char*     dst;
	socklen_t size;

	peer_addr_len = sizeof(peer_sockaddr);
	
	if (VERBOSE_MODE)
	{
		printf(":: Fetching peer information\n");
	}
	
	if (getpeername(client_fd, (struct sockaddr*) &peer_sockaddr, &peer_addr_len) == -1)
	{
		fprintf(stderr,
			"%s: unable to get peer name: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	switch (peer_sockaddr.ss_family)
	{
		case AF_INET:
			printf("IPv4 connection!\n");
			ipv4 = ;
			port = ;
			break;
		case AF_INET6:
			printf("IPv6 connection!\n");
			ipv6 = ;
			port = ;
			break;
		default:
			// ERROR
			printf("Unknown connection!\n");
			break;
	}
	
	port = ntohs(netshort);
	inet_ntop(af, src, dst, size);
}*/
