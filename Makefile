include library.mk

PROBLEM_SETS := $(shell find . -maxdepth 1 -type d -name "ps*")

# Detect nix-shell, use it if present
NIX_SHELL := $(shell command -v nix-shell 2> /dev/null)
SUBMAKE := $(MAKE) -e -C $$$$ps || exit 1
ifdef NIX_SHELL
ifndef IN_NIX_SHELL
SUBMAKE := nix-shell --run "$(SUBMAKE)"
endif
endif

# This function calls `make` in each subdirectory of the form ps* using the
# provided command, and passing down variables from this instance.
#
# $$$$ is necessary in a macro instead of $$
# TODO: this doesn't really work.
define forall =
$(1):;
	for ps in $(PROBLEM_SETS); do $(SUBMAKE); done
endef

$(call forall,all)
$(call forall,build)
$(call forall,test)
$(call forall,clean)
