.DEFAULT_GOAL := all

.PHONY: all
all: build test clean

.PHONY: build
build: divisors high-low

.PHONY: test
test:; exit 0

define build =
$(1): $(1).o;
	$(CC) -lm $(1).o $(CFLAGS)
endef

# targets
$(call build,divisors)
$(call build,high-low)

.PHONY: clean
clean:
	rm a.out
	rm -f *.o
	rm -rf
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
