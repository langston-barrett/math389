.DEFAULT_GOAL := all

include ../../library.mk

.PHONY: all build test clean
all: build test clean

%.gen.h: %.c ../../makeheaders.sh $(DEPS)
	bash ../../makeheaders.sh $<;

build: cache-penalty.gen.h cache-penalty.o cache-size.gen.h cache-size.o csv.gen.h csv.o main.o random-array.o random-array.gen.h
	$(CC) $(CFLAGS) -o cache -lm cache-penalty.o cache-size.o csv.o random-array.o main.o

analysis: build
	#nix-shell .
	[[ -f cache-size-data.csv ]] || ./cache
	R --no-save --no-restore -e "require(knitr); knit('analysis.Rtex')"
	lualatex -shell-escape analysis.tex

penalty: buildpenalty
	#nix-shell .
	[[ -f cache-penalty-data.csv ]] || ./penalty
	R --no-save --no-restore -e "require(knitr); knit('penalty.Rtex')"
	lualatex penalty.tex

test: csv-test random-array-test

$(call test,csv)
$(call test,random-array)

