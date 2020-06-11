all: gameoflife.o gol.o gol.h libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 -o gameoflife gameoflife.o -lgol -L.

libgol.so: gol.o
	gcc -Wall -Wextra -pedantic -std=c11 -shared gol.o -o libgol.so

gameoflife.o: gameoflife.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -c gameoflife.c -fPIC

gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -c gol.c -fPIC

clean:
	rm -rf gameoflife gameoflife.o gol.o libgol.so
