.DEFAULT_GOAL := all

include ../../library.mk

.PHONY: all build test clean
all: build test clean

%.gen.h: %.c ../../makeheaders.sh $(DEPS)
	bash ../../makeheaders.sh $<;

build:
	exit 0

test: csv-test

$(call test,csv)
