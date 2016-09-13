.DEFAULT_GOAL := all

PROBLEM_SETS := $(shell find . -maxdepth 1 -type d -name "ps*")

.PHONY: all
all:
	for ps in $(PROBLEM_SETS); do \
		$(MAKE) -e -C $$ps || exit 1; \
	done
