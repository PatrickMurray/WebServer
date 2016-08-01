NAME    := webserver
SRC     := main.c arguments.c server.c request.c
HEADERS := headers.h

CFLAGS      := -Wall -Werror -Wpedantic -g3
OBJ         := $(SRC:.c=.o)
LINUX_LINKS := -lbsd -pthread

all: $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LINUX_LINKS) $(CFLAGS)

clean:
	-$(RM) *~
	-$(RM) \#*
	-$(RM) *.o
	-$(RM) *.core

fclean:	clean
	-$(RM) $(NAME)


re: fclean all
