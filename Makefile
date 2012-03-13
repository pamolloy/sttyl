CC=gcc
CFLAGS=-Wall -g -include "defs.h"
OBJECTS=main.o special.o setting.o speed.o

sttyl: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS)

$(OBJETS) : defs.h

.PHONY : clean
clean:
	rm sttyl $(OBJECTS)
