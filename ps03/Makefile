.DEFAULT_GOAL := all

.PHONY: all
all: turtle

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.h: %.c $(DEPS)
	bash ../makeheaders.sh $<

turtle: turtle.h turtle.o
	$(CC) -lm turtle.o

.PHONY: clean
clean:
	rm a.out
	rm -f *.o
	rm -f *.aux *.fdb_latexmk *.fls *.log *.out *.synctex.gz
