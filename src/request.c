#include "headers.h"


void* handle_request(void* fd_ptr)
{
	int   client_fd;
	char* buffer;
	char* line;
	int   loop;
	char* response;

	client_fd = (intptr_t) fd_ptr;
	
	if ((buffer = calloc(HEADER_BUFFER_LENGTH, sizeof(char))) == NULL)
	{
		fprintf(stderr,
			"%s: unable to allocate memory\n",
			getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	loop = 1;
	while (loop)
	{
		line = request_getline(client_fd, buffer, HEADER_BUFFER_LENGTH);
		
		if (strlen(line) == 0)
		{
			loop = 0;
		}
		
		free(line);
	}
	
	free(buffer);
	
	response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 44\r\n\r\nHello World, I am a simple response message!";

	if (send(client_fd, response, strlen(response), 0) == -1)
	{
		fprintf(stderr,
			"%s: unable to send response: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	if (close(client_fd) == -1)
	{
		fprintf(stderr,
			"%s: unable to close client connection: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}

	pthread_exit((void*) (intptr_t) EXIT_SUCCESS);
}


char* request_getline(int client_fd, char* buffer, size_t buffer_length)
{
	char*   line;
	size_t  line_terminator_idx;
	size_t  init_line_len;
	int     found;
	ssize_t recv_len;
	//size_t  idx;
	char*   position;
	size_t  shift;
	
	/* Create the initial line */
	if ((line = calloc(1, sizeof(char))) == NULL)
	{
		fprintf(stderr,
			"%s: unable to allocate memory\n",
			getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	line_terminator_idx       = 0;
	line[line_terminator_idx] = '\0';
	found                     = 0;

	while (!found)
	{
		init_line_len = strlen(line);

		/* PEEK FROM THE SOCKET */
		memset(buffer, 0, buffer_length);
		if ((recv_len = recv(client_fd, buffer, buffer_length, MSG_PEEK)) < 0)
		{
			fprintf(stderr,
				"%s: unable to recv client socket: %s\n",
				getprogname(),
				strerror(errno)
			);
			exit(EXIT_FAILURE);
		}
		else if (recv_len == 0)
		{
			break;
		}
		
		/* ALLOCATE SPACE IN THE LINE FOR THE PEEK */
		if ((line = realloc(line, sizeof(char) * (init_line_len + recv_len + 1))) == NULL)
		{
			fprintf(stderr,
				"%s: unable to reallocate memory\n",
				getprogname()
			);
			exit(EXIT_FAILURE);
		}
		
		/* CONCATENATE THE PEEK TO THE END OF THE LINE
		for (idx = 0; idx < recv_len; idx++)
		{
			line[line_terminator_idx++] = buffer[idx];
		}
		*/
		
		memcpy(&line[line_terminator_idx], buffer, sizeof(char) * recv_len);
		line_terminator_idx += recv_len;

		line[line_terminator_idx] = '\0';

		/* SEARCH FOR A NEWLINE IN THE LINE */
		if ((position = strstr(line, "\r\n")) != NULL)
		{
			found               = 1;
			line_terminator_idx = position - line;
		}
		
		line[line_terminator_idx] = '\0';

		/**/
		if ((line = realloc(line, sizeof(char) * (line_terminator_idx + 1))) == NULL)
		{
			fprintf(stderr,
				"%s: unable to reallocate memory\n",
				getprogname()
			);
			exit(EXIT_FAILURE);
		}

		if (found)
		{
			shift = strlen(line) - init_line_len + 2;
		}
		else
		{
			shift = recv_len;
		}
		
		memset(buffer, 0, buffer_length);
		if ((recv_len = recv(client_fd, buffer, shift, 0)) < 0)
		{
			fprintf(stderr,
				"%s: unable to recv client socket: %s\n",
				getprogname(),
				strerror(errno)
			);
			exit(EXIT_FAILURE);
		}
		else if (recv_len == 0)
		{
			break;
		}
	}
	
	return line;
}
