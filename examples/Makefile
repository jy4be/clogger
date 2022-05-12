CC = gcc

IDIR = ..

CFLAGS = -Og -Wall -I $(IDIR)
LDFLAGS = -Og

OBJS = ../clogger.o example.o

PROG = logger_examples

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(PROG) $(OBJS)
