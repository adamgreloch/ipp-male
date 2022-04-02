CC = gcc
CFLAGS = -w -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2 -g

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

.PHONY: clean

all: labyrinth

labyrinth: $(OBJ)
	$(CC) $(CFLAGS) -o labyrinth $^

main.o: labyrinth.h
labyrinth.o: labyrinth.h input.h stack.h
bitTable.o: bitTable.h input.h
cubes.o: cubes.h input.h
stack.o: stack.h
input.o: input.h
errMem.o: errMem.h

clean:
	rm *.o labyrinth
