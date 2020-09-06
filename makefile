CC=g++

all: main.o chip.o
	$(CC) -O3 -o chip main.o chip.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main.o:
	$(CC) -O3 -c src/main.cpp

chip.o:
	$(CC) -O3 -c src/chip.cpp

.PHONY: clean

clean:
	rm -f chip main.o chip.o
