CC = gcc
CFLAGS = -Wall -m64
ALLEGROFLAGS= $$(pkg-config --libs allegro-5)
all: main.o julia.o
	$(CC) $(CFLAGS) -o example main.o julia.o

julia.o: julia.s
	nasm -f elf64 -o julia.o julia.s

main.o: main.c
	$(CC) main.c -o main.o $$(pkg-config --CFLAGS --libs allegro-5 allegro_primitives-5)

clean:
	rm -f *.o

