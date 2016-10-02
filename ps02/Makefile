include ../library.mk

.PHONY: build
build: bit-palindromes decimal-sum mergesort transpose

.PHONY: test
test: bit-palindromes-test decimal-sum-test mergesort-test

# targets
$(call build-main,bit-palindromes)
$(call build-main,decimal-sum)
$(call build-main,mergesort)
$(call build,transpose)

$(call test,bit-palindromes)
$(call test,decimal-sum)
$(call test,mergesort)
