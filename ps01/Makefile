.DEFAULT_GOAL := all

CFLAGS=-std=gnu99
export CFLAGS

.PHONY: all
all: tex divisors high-low clean

tex: ps01.tex
	echo "TODO"

divisors: divisors.c
	gcc divisors.c

high-low: high-low.c
	gcc high-low.c

.PHONY: clean
clean:
	rm a.out
	rm -r auto/ &> /dev/null || true
	rm *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz &> /dev/null || true
