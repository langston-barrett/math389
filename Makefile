.DEFAULT_GOAL := all

PROBLEM_SETS := $(shell find . -maxdepth 1 -type d -name "ps*")

# Detect nix-shell, use it if present
NIX_SHELL := $(shell command -v nix-shell 2> /dev/null)
SUBMAKE := $(MAKE) -e -C $$$$ps $(1) || exit 1
ifdef NIX_SHELL
SUBMAKE := nix-shell --command "$(SUBMAKE)"
endif

export CFLAGS?=-std=gnu11

# This function calls `make` in each subdirectory of the form ps* using the
# provided command, and passing down variables from this instance.
#
# $$$$ is necessary in a macro instead of $$
define forall =
$(1):;
	for ps in $(PROBLEM_SETS); do $(SUBMAKE); done
endef

$(call forall,all)
$(call forall,build)
$(call forall,test)
$(call forall,clean)
