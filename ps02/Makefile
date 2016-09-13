.DEFAULT_GOAL := all

CFLAGS=-std=gnu99
export CFLAGS

.PHONY: all
all: tex bit-palindromes decimal-sum mergesort transpose clean

tex: ps02.tex
	echo "TODO"

bit-palindromes: bit-palindromes.c
	gcc bit-palindromes.c

decimal-sum: decimal-sum.c
	gcc decimal-sum.c

mergesort: mergesort.c
	gcc mergesort.c

transpose: transpose.c
	gcc transpose.c

.PHONY: clean
clean:
	rm a.out
	rm -r auto/ &> /dev/null || true
	rm *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz &> /dev/null || true
