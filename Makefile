.DEFAULT_GOAL := all

PROBLEM_SETS := $(shell find . -maxdepth 1 -type d -name "ps*")
export CFLAGS?=-std=gnu99 # Travis doesn't support C11 yet.

# $$$$ is necessary in a macro instead of $$
define forall =
$(1):;
	for ps in $(PROBLEM_SETS); do \
		$(MAKE) -e -C $$$$ps $(1) || exit 1; \
	done
endef

$(call forall,all)
$(call forall,build)
$(call forall,test)
$(call forall,clean)
