include ../library.mk

.PHONY: build
build: bstree

.PHONY: test
test: llist-test hash-table-test bstree-test

# These are build/test rules, implemented entirely in functions
$(call test,llist)
$(call test2,hash-table,llist)
$(call build-main,bstree)
$(call test,bstree)
