CC = gcc
CFLAGS = -w -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O1 -g

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(addprefix build/, $(OBJ))
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/labyrinth $^

build/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm build/*
