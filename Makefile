labirynth:
	@mkdir -p build
	gcc -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2 -o build/labirynth labirynth.c

clean:
	rm labirynth
