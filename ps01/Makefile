.DEFAULT_GOAL := all

.PHONY: all
all: tex divisors high-low clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tex: ps01.tex
	echo "TODO"

divisors: divisors.o
	$(CC) -lm divisors.o

high-low: high-low.o
	$(CC) -lm high-low.o

.PHONY: clean
clean:
	rm a.out
	rm -rf
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
