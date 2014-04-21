CC = gcc
CFLAGS = -g -Wall -pthread 

PROGRAMS = fumantes

all: $(PROGRAMS)

clean:
	rm -f *.o *~ $(PROGRAMS)