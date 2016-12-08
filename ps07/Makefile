include ../library.mk

CFLAGS := -g

.PHONY: build
build: sand

.PHONY: test
test: sand-test

.PHONY: frama-c
frama-c:
	frama-c -wp sand.h sand.c sand-main.c

# These are build/test rules, implemented entirely in functions
$(call test,sand)
$(call build-main,sand)
