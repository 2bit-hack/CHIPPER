CC=g++

all: main.o chip.o graphics.o
	$(CC) -o chip main.o chip.o graphics.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

main.o:
	$(CC) -c src/main.cpp

chip.o:
	$(CC) -c src/chip.cpp

graphics.o:
	$(CC) -c src/graphics.cpp

.PHONY: clean

clean:
	rm -f chip main.o chip.o graphics.o