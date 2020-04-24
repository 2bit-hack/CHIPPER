IDIR=includes/
SDIR=src/

CC=g++

all: main.o chip.o
	$(CC) -o chip main.o chip.o

main.o:
	$(CC) -c src/main.cpp

chip.o:
	$(CC) -c src/chip.cpp

.PHONY: clean

clean:
	rm -f chip main.o chip.o