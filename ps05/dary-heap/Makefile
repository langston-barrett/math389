.DEFAULT_TARGET=all

.PHONY: all build test clean
all: build test clean

build: dary-heap-main.o dary-heap.o
	$(CC) $(CFLAGS) -o dary-heap -lm dary-heap-main.o dary-heap.o

test: dary-heap-test.o dary-heap.o
	$(CC) $(CFLAGS) -o test -lm dary-heap-test.o dary-heap.o
	./test
	rm -f test
