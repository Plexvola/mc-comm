CC=clang
CFLAGS=-Wall -g -D_DEBUG

mcping: mcping.c mctypes.o mcnetwork.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

lint:
	indent -kr -ts4 *.c

clean:
	rm -f *.o *~
