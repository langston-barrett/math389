.DEFAULT_GOAL := all

.PHONY: all
all: test clean

.PHONY: test
test: llist-test hash-table-test

.PHONY: clean
clean:
	rm -f a.out
	rm -f *.o
	rm -f *.gen.h
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz

%.gen.h: %.c ../makeheaders.sh $(DEPS)
	bash ../makeheaders.sh $<;

# TODO: rule that constructs fake main when needed

# Build an arbitrary binary from <name>.o <name>-main.o and <name>.gen.h
define build =
$(1): $(1).gen.h $(1).o $(1)-main.o;
	$(CC) -lm $(1).o $(1)-main.o $(CFLAGS)
endef

define build2 =
$(1): $(2).gen.h $(2).o $(1).gen.h $(1).o $(1)-main.o;
	$(CC) -lm $(1).o $(1)-main.o $(2).o $(CFLAGS)
endef

# Build a test binary from <name>.o <name>-test.o and <name>.gen.h
define test =
$(1)-test: $(1).gen.h $(1).o $(1)-test.o;
	$(CC) -lm $(1).o $(1)-test.o $(CFLAGS)
	./a.out
	rm a.out
endef

# Build a test binary with some extra files
define test2 =
$(1)-test: $(2).gen.h $(2).o $(1).gen.h $(1).o $(1)-test.o;
	$(CC) -lm $(1).o $(1)-test.o $(2).o $(CFLAGS)
	./a.out
	rm a.out
endef

# These are build/test rules, implemented entirely in functions
$(call build,turtle)
$(call test,llist)
$(call test2,hash-table,llist)
