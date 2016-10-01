.DEFAULT_TARGET=all

CFLAGS ?= -g

.PHONY: all build test clean
all: clean build

build: main.o dc.s lib.o
	$(CC) -c dc.s $(CFLAGS)
	$(CC) -o dc -lm main.o dc.o lib.o $(CFLAGS) # dclib.s

test: test.o dc.s lib.o
	$(CC) -o test -lm dc.o test.o lib.o $(CFLAGS) #dclib.s
	./test

clean:
	rm -f *.o
	rm -f dc
