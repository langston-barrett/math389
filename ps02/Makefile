.DEFAULT_GOAL := all

.PHONY: all
all: build test clean

.PHONY: build
build: bit-palindromes decimal-sum mergesort transpose graph clean

.PHONY: test
test:; exit 0

define build =
$(1): $(1).o;
	$(CC) -lm $(1).o $(CFLAGS)
endef

# targets
$(call build,bit-palindromes)
$(call build,decimal-sum)
$(call build,mergesort)
$(call build,transpose)

graph:
	echo "TODO"

.PHONY: clean
clean:
	rm a.out
	rm -f *.o
	rm -rf auto/
	rm -f transpose.txt
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
