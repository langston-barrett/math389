.DEFAULT_GOAL := all

CFLAGS=-std=c11
export CFLAGS

.PHONY: all
all:
	for ps in ps01 ps02; do \
		cd $$ps; \
		$(MAKE); \
		cd -; \
	done
