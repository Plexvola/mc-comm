CC=clang
CFLAGS=-Wall -g

mcping: mcping.c mctypes.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

lint:
	indent -kr -ts4 *.c

clean:
	rm -f *.o *~
