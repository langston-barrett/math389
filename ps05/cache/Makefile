.DEFAULT_GOAL := all

include ../../library.mk

%.gen.h: %.c ../../makeheaders.sh $(DEPS)
	bash ../../makeheaders.sh $<;

build: random-array.gen.h cache-size.gen.h csv.gen.h cache-size.o csv.o random-array.o main
	$(CC) $(CFLAGS) -o cache -lm cache-size.o csv.o random-array.o main.o

analysis: build
	[[ -f cache-size-data.csv ]] || ./cache
	R --no-save --no-restore -e "require(knitr); knit('analysis.Rtex')"
	lualatex -shell-escape analysis.tex

test: csv-test random-array-test

$(call test,csv)
$(call test,random-array)

