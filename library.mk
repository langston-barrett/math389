# Shared functions for use in sub-makefiles

.DEFAULT_GOAL := all

.PHONY: all
all: build test clean

# Generate header file for functions
%.gen.h: %.c ../makeheaders.sh $(DEPS)
	bash ../makeheaders.sh $<;

define build =
$(1): $(1).o $(DEPS);
	$(CC) $(CFLAGS) -lm $(1).o
endef

# Build an arbitrary binary from <name>.o <name>-main.o and <name>.gen.h
define build-main =
$(1): $(1).gen.h $(1).o $(1)-main.o $(DEPS);
	$(CC) $(CFLAGS) -lm $(1).o $(1)-main.o
endef

# Build a binary with some extra files
define build-main-2 =
$(1): $(2).gen.h $(2).o $(1).gen.h $(1).o $(1)-main.o $(DEPS);
	$(CC) $(CFLAGS) -lm $(1).o $(1)-main.o $(2).o
endef

# Build a test binary from <name>.o <name>-test.o and <name>.gen.h
define test =
$(1)-test: $(1).gen.h $(1).o $(1)-test.o $(DEPS);
	$(CC) $(CFLAGS) -lm $(1).o $(1)-test.o
	./a.out
	rm a.out
endef

# Build a test binary with some extra files
define test2 =
$(1)-test: $(2).gen.h $(2).o $(1).gen.h $(1).o $(1)-test.o $(DEPS);
	$(CC) $(CFLAGS) -lm $(1).o $(1)-test.o $(2).o
	./a.out
	rm a.out
endef

# Generic clean
.PHONY: clean
clean:
	rm -f a.out
	rm -f *.o
	rm -f *.bc
	rm -f *.plist
	rm -f *.gen.h
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
	rm -f *.png
	rm -f klee-last
	rm -rf klee-out-*
	find . -type f -executable -delete
