#include "headers.h"


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
		tokens[*size] = strdup(token);
		(*size)++;

		if ((tokens = realloc(tokens, sizeof(char*) * ((*size) + 1))) == NULL)
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
