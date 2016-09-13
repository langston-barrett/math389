.DEFAULT_GOAL := all

.PHONY: all
all:
	for ps in ps01 ps02; do \
		cd $$ps; \
		$(MAKE); \
		cd -; \
	done
