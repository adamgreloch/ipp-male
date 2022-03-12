CC = gcc
CFLAGS = -w -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2

SRC=$(wildcard *.c)

labirynth: $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o build/$@

clean:
	rm build/labirynth
