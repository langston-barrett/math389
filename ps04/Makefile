.DEFAULT_TARGET=all

# TODO: remove
CFLAGS ?= -g

.PHONY: all build test clean
all: clean build

build: main.o dc.s lib.o
	$(CC) -c dc.s $(CFLAGS)
	$(CC) -o dc -lm main.o dc.o lib.o $(CFLAGS)

test: dc-test.o dc.s lib.o
	$(CC) -c dc.s $(CFLAGS)
	$(CC) -o test -lm dc-test.o dc.o lib.o $(CFLAGS)
	./test

clean:
	rm -f *.o
	rm -f *.hi
	rm -f dc
	rm -f test
