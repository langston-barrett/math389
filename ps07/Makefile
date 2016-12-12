include ../library.mk

export CC ?= clang
export CFLAGS ?= -g -pthread -Werror -Weverything -std=gnu11 -Qunused-arguments
KLEE_VERSION := 1.2.0
KLEE_MAX_MEM := 2048

############################ Setup ############################ 
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<;

%.gen.h: %.c ../makeheaders.sh $(DEPS)
	bash ../makeheaders.sh $<;

.PHONY: build
build: sand tsand

############################ Binaries ############################

sand-lib: sand-lib.h sand-lib.gen.h sand-lib.o
	$(CC) $(CFLAGS) -c sand-lib.c;

sand: sand-lib sand.h sand.o sand-main.o
	$(CC) $(CFLAGS) -o sand -lm sand-main.o sand.o sand-lib.o

tsand: sand tsand.o
	$(CC) $(CFLAGS) -DTHREADS -o tsand -lm sand-main.o sand.o tsand.o sand-lib.o

############################ Tests ############################
test: sand-lib-test sand-test

sand-lib-test: sand-lib sand-lib-test.o
	$(CC) $(CFLAGS) -o test-lib -lm sand-lib-test.o sand-lib.o

sand-test: sand tsand sand-test.o
	$(CC) $(CFLAGS) -o test -lm sand-test.o sand.o tsand.o sand-lib.o

############################ Release/Analysis ############################
csv: ../ps05/cache/csv.gen.h
	$(CC) $(CFLAGS) -o ../ps05/cache/csv.o -c ../ps05/cache/csv.c

data-main: csv sand-lib tsand data-main.o
	$(CC) $(CFLAGS) -o data -lm data-main.o sand.o tsand.o sand-lib.o ../ps05/cache/csv.o

data: data-main
	[[ -f data.csv ]] || ./data
	R --no-save --no-restore -e "require(knitr); knit('analysis.Rtex')"
	lualatex -shell-escape analysis.tex

############################ Static Analysis ############################


%.bc: %.c $(DEPS)
	clang -I/home/klee/klee_src/include -emit-llvm -g -c $<;

# https://developer.mozilla.org/en-US/docs/Mozilla/Testing/Clang_static_analysis
clang:
  find . -type f -name "*.c" -exec $(CC) $(CFLAGS) --analyze {} +

klee-sand: sand sand-lib.bc sand.bc sand-klee-main.bc
	llvm-link -o klee.bc sand-klee-main.bc sand-lib.bc sand.bc
	klee --allow-external-sym-calls --max-memory $(KLEE_MAX_MEM) --optimize --libc=uclibc --posix-runtime klee.bc --max-fail 0
	ktest-tool --write-ints klee-last/*.ktest

klee-shell:
	sudo docker run -v "$(PWD)/../:/data:z" --rm -ti --ulimit='stack=-1:-1' \
    "klee/klee:$(KLEE_VERSION)" /bin/bash -c "cd /data/ps07; exec bash"

klee:
	sudo docker pull "klee/klee:$(KLEE_VERSION)"
	sudo docker run -v "$(PWD)/../:/data:z" --rm -ti --ulimit='stack=-1:-1' \
    "klee/klee:$(KLEE_VERSION)" \
    /bin/bash -li -c "make -C /data/ps07 klee-sand"

# http://valgrind.org/docs/manual/quick-start.html
valgrind: test
	valgrind ./test-lib
	valgrind ./test
