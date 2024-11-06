CC = gcc
CFLAGS = -Wall -Werror -g -std=c99
INCLUDES = SDL2 


build:
	$(CC) $(CFLAGS) -o snake *c -l$(INCLUDES)
run:
	make build && ./snake
