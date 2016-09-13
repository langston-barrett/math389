.DEFAULT_GOAL := all

.PHONY: all
all: tex bit-palindromes decimal-sum mergesort transpose clean

tex: ps02.tex
	echo "TODO"

bit-palindromes: bit-palindromes.c
	$(CC) bit-palindromes.c

decimal-sum: decimal-sum.c
	$(CC) decimal-sum.c

mergesort: mergesort.c
	$(CC) mergesort.c

transpose: transpose.c
	$(CC) transpose.c

.PHONY: clean
clean:
	rm a.out
	rm -r auto/ &> /dev/null || true
	rm *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz &> /dev/null || true
