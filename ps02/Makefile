.DEFAULT_GOAL := all

.PHONY: all
all: tex bit-palindromes decimal-sum mergesort transpose graph clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tex: ps02.tex
	echo "TODO"

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
	rm -f *.o
	rm -rf auto/
	rm -f transpose.txt
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
