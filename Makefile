.DEFAULT_GOAL := all

CFLAGS=-std=gnu11
export CFLAGS

.PHONY: all
all:
	for ps in ps01 ps02; do \
		$(MAKE) -C $$ps || break; \
	done
