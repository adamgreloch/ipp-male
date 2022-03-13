CC = gcc
CFLAGS = -w -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O1 -g

SRC=$(wildcard *.c)

all: $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o build/labyrinth

clean:
	rm build/*
