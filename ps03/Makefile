include ../library.mk

.PHONY: build
build: turtle

.PHONY: test
test: llist-test hash-table-test

# These are build/test rules, implemented entirely in functions
$(call build-main,turtle)
$(call test,llist)
$(call test2,hash-table,llist)
