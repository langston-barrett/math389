.DEFAULT_TARGET=all

.PHONY: all build test clean
all: build test clean

build: main.o dc.s lib.o
	$(CC) -c dc.s $(CFLAGS)
	$(CC) -o dc -lm main.o dc.o lib.o $(CFLAGS)

test: dc-test.o dc.s lib.o
	$(CC) -c dc.s $(CFLAGS)
	$(CC) -o test -lm dc-test.o dc.o lib.o $(CFLAGS)
	./test

asmfmt:
	mkdir gopath
	export GOPATH=$PWD/gopath
	go get -u github.com/klauspost/asmfmt/cmd/asmfmt
	mv gopath/bin/asmfmt .
	rm -rf gopath

clean:
	rm -f *.o
	rm -f *.hi
	rm -f dc
	rm -f test
