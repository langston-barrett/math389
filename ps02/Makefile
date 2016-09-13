.DEFAULT_GOAL := all

export CFLAGS=-std=gnu99

.PHONY: all
all: tex bit-palindromes decimal-sum mergesort transpose graph clean

tex: ps02.tex
	echo "TODO"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bit-palindromes: bit-palindromes.o
	$(CC) -lm bit-palindromes.o

decimal-sum: decimal-sum.o
	$(CC) -lm decimal-sum.o

mergesort: mergesort.o
	$(CC) -lm mergesort.o

transpose: transpose.o
	$(CC) -lm transpose.o

graph:
	echo "TODO"

.PHONY: clean
clean:
	rm a.out
	rm -rf auto/
	rm -f transpose.txt
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
