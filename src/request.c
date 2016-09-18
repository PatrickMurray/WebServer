#include "headers.h"


void* handle_request(void* fd_ptr)
{
	int    client_fd;
	char*  buffer;
	char*  line;
	int    loop;
	char*  response;
	int    line_num;
	char** tokens;
	size_t tokens_size;
	size_t idx;

	client_fd = (intptr_t) fd_ptr;
	
	if ((buffer = calloc(HEADER_BUFFER_LENGTH, sizeof(char))) == NULL)
	{
		fprintf(stderr,
			"%s: unable to allocate memory\n",
			getprogname()
		);
		exit(EXIT_FAILURE);
	}
	
	loop     = 1;
	line_num = 0;
	while (loop)
	{
		line = request_getline(client_fd, buffer, HEADER_BUFFER_LENGTH);

		if (strlen(line) == 0)
		{
			loop = 0;
		}

		if (line_num == 0)
		{
			printf("%s\n", line);
			tokens_size = 0;
			tokens = tokenize(line, " ", &tokens_size);
			if (tokens != NULL)
			{
				for (idx = 0; idx < tokens_size; idx++)
				{
					printf("%s\n", tokens[idx]);
				}
				free_tokens(tokens, tokens_size);
			}
		}
		
		free(line);
		line_num++;
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


char** tokenize(char* string, char* delimiter, size_t* size)
{
	char** tokens;
	char*  str;
	char*  saveptr;
	char*  token;

	*size = 0;
	
	if (strlen(string) == 0)
	{
		return NULL;
	}
	
	if ((tokens = calloc(1, sizeof(char*))) == NULL)
	{
		fprintf(stderr,
			"%s: unable to allocate memory\n",
			getprogname()
		);
		exit(EXIT_FAILURE);
	}

	if ((str = strdup(string)) == NULL)
	{
		fprintf(stderr,
			"%s: unable to duplicate string: %s\n",
			getprogname(),
			strerror(errno)
		);
		exit(EXIT_FAILURE);
	}
	
	saveptr = NULL;
	token   = strtok_r(str, delimiter, &saveptr);

	while (token != NULL) {
		tokens[*size] = strdup(token); // INVALID WRITE OF SIZE 8
		(*size)++;

		if ((tokens = realloc(tokens, sizeof(char*) * ((*size) + 1))) == NULL) /* ADDRESS ?????? IS 6 BYTES AFTER A BLOCK OF SIZE */
		{
			fprintf(stderr,
				"%s: unable to reallocate memory\n",
				getprogname()
			);
			exit(EXIT_FAILURE);
		}
		
		token = strtok_r(NULL, delimiter, &saveptr);
	}
	
	/* REALLOCATE DOWN 1 */
	if ((tokens = realloc(tokens, sizeof(char*) * (*size))) == NULL)
	{
		fprintf(stderr,
			"%s: unable to reallocate memory\n",
			getprogname()
		);
		exit(EXIT_FAILURE);
	}

	free(str);

	return tokens;
}

void free_tokens(char** tokens, size_t size)
{
	size_t idx;

	for (idx = 0; idx < size; idx++)
	{
		free(tokens[idx]);
	}
	free(tokens);
}
