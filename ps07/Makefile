include ../library.mk

# Extra CFLAGS. Appending doesn't work??
FLG := -g -pthread
KLEE_VERSION := 1.2.0
KLEE_MAX_MEM := 2048

############################ Setup ############################ 
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(FLG) -c $<;

%.gen.h: %.c ../makeheaders.sh $(DEPS)
	bash ../makeheaders.sh $<;

############################ Binaries ############################
build: sand tsand

sand-lib: sand-lib.h sand-lib.o
	$(CC) $(CFLAGS) $(FLG) -c sand-lib.c;

sand: sand-lib sand.h sand.o sand-main.o
	$(CC) $(CFLAGS) $(FLG) -o sand -lm sand-main.o sand.o sand-lib.o

tsand: sand tsand.o
	$(CC) $(CFLAGS) $(FLG) -DTHREADS -o tsand -lm sand-main.o sand.o tsand.o sand-lib.o

############################ Tests ############################
# All tests will also Valgrind the test binaries.
# http://valgrind.org/docs/manual/quick-start.html
TMP_FILE := $(shell mktemp)

test: sand-lib-test sand-test

define valgrind =
	valgrind --tool=$(2) $(1) &> $(TMP_FILE) || (cat $(TMP_FILE); exit 1);
	grep -q "ERROR SUMMARY: 0 errors" $(TMP_FILE) || (cat $(TMP_FILE); exit 1);
	rm -rf $(TMP_FILE)
endef
#ifdef CI
define valgrind =
	echo "not running valgrind in ci build"
	$(1) || exit 1;
endef
#endif

sand-lib-test: sand-lib sand-lib-test.o
	$(CC) $(CFLAGS) $(FLG) -o test-lib -lm sand-lib-test.o sand-lib.o;
	$(call valgrind,./test-lib,memcheck)

sand-test: sand tsand sand-test.o
	$(CC) $(CFLAGS) $(FLG) -o test -lm sand-test.o sand.o tsand.o sand-lib.o;
	$(call valgrind,./test,memcheck)

# Test for threading errors. Not very helpful right now.
sand-test-thread: sand-test
	$(call valgrind,./test,drd)

############################ Release/Analysis ############################
csv: ../ps05/cache/csv.gen.h
	$(CC) $(CFLAGS) $(FLG) -o ../ps05/cache/csv.o -c ../ps05/cache/csv.c

data-main: csv sand-lib tsand data-main.o
	$(CC) $(CFLAGS) $(FLG) -o data -lm data-main.o sand.o tsand.o sand-lib.o ../ps05/cache/csv.o

data: data-main
	[[ -f data.csv ]] || ./data
	R --no-save --no-restore -e "require(knitr); knit('analysis.Rtex')"
	lualatex -shell-escape analysis.tex

############################ Static Analysis ############################

%.bc: %.c $(DEPS)
	clang -I/home/klee/klee_src/include -emit-llvm -g -c $<;

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
