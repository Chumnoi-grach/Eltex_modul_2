CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

all: bin/main.o bin/math.o
	$(CC) bin/main.o bin/math.o -o bin/calculator -lm

bin/calculator: bin/main.o bin/math.o
	$(CC) bin/main.o bin/math.o -o bin/calculator

bin/main.o: main.c dir
	$(CC) $(CFLAGS) -c main.c -o bin/main.o

bin/math.o: math.c dir
	$(CC) $(CFLAGS) -c math.c -o bin/math.o

dir:
	mkdir -p bin