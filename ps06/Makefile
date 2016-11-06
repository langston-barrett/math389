.DEFAULT_GOAL := all

CFLAGS := -g

.PHONY: all build test clean
all: build test clean

%.gen.h: %.c ../makeheaders.sh $(DEPS)
	bash ../makeheaders.sh $<;

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<;

# Binaries
client: client.c solitaire-lib.c
	$(CC) $(CFLAGS) -c client.c
	$(CC) $(CFLAGS) -o client -lm client.o

server: server.c solitaire-lib.c
	$(CC) $(CFLAGS) -c server.c
	$(CC) $(CFLAGS) -o server -lm server.o

build: client server

#test:

.PHONY: clean
clean:
	rm -f a.out
	rm -f *.o
	rm -f *.gen.h
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
