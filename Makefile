CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic

.PHONY: all clean

all: task1 task2

task1: task1.c
	$(CC) $(CFLAGS) -o task1 task1.c

task2: task2.c
	$(CC) $(CFLAGS) -o task2 task2.c

clean:
	rm -f task1 task2 *.o
