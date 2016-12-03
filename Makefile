CC=gcc
CFLAGS=-std=c99 -Wall -O3 -g

shsh: shsh.c
	$(CC) $(CFLAGS) shsh.c -o shsh

.PHONY: clean
clean:
	rm shsh
	rm -rf shsh.dSYM

