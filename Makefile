.DEFAULT_GOAL := all

PROBLEM_SETS := $(shell find . -maxdepth 1 -type d -name "ps*")

CFLAGS=-std=gnu11
export CFLAGS

.PHONY: all
all:
	for ps in $(PROBLEM_SETS); do \
		$(MAKE) -C $$ps || exit 1; \
	done
