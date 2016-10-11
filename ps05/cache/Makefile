.DEFAULT_GOAL := all

include ../../library.mk

.PHONY: all build test clean
all: build test clean

%.gen.h: %.c ../../makeheaders.sh $(DEPS)
	bash ../../makeheaders.sh $<;

build: cache-penalty.gen.h cache-penalty.o cache-size.gen.h cache-size.o csv.gen.h csv.o main.o
	$(CC) $(CFLAGS) -o cache -lm cache-penalty.o cache-size.o csv.o main.o

analysis:
	#nix-shell .
	R --no-save --no-restore -e "require(knitr); knit('analysis.Rtex')"
	lualatex analysis.tex

test: csv-test


$(call test,csv)
