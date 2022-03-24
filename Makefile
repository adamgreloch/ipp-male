# https://www.gnu.org/software/make/manual/html_node/Complex-Makefile.html#Complex-Makefile

CC = gcc
CFLAGS = -w -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O0 -g

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

#all: $(addprefix build/, $(OBJ))
all: $(addprefix build/, $(OBJ))
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/labyrinth $^

build/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm build/*
