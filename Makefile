CC = gcc
CFLAGS=-Wall -g

sttyl: sttyl.c
	$(CC) sttyl.c

clean:
	rm -f sttyl
