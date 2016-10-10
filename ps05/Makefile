.DEFAULT_GOAL := all

.PHONY: all build test clean

define forall =
$(1):;
	#for dir in cache dary-heap; do
	for dir in cache; do \
		$(MAKE) -e -C $$$$dir $(1) || exit 1; \
	done
endef

$(call forall,all)
$(call forall,build)
$(call forall,test)
$(call forall,clean)
