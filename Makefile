CC=gcc
CFLAGS=-Wall -g
OBJECTS=main.o special.o setting.o speed.o

sttyl: $(OBJECTS)
	$(CC) $(CFLAGS) -o sttyl $(OBJECTS)

$(OBJETS) : defs.h

.PHONY : clean
clean:
	rm sttyl $(OBJECTS)
