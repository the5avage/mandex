all: release debug

run:
	cd bin/release && $(MAKE) run

run_debug:
	cd bin/debug && $(MAKE) run

release:
	cd bin/release && $(MAKE) all

debug:
	cd bin/debug && $(MAKE) all

.PHONY: clean_release
clean_release:
	cd bin/release && $(MAKE) clean

.PHONY: clean_debug
clean_debug:
	cd bin/debug && $(MAKE) clean

.PHONY: clean
clean: clean_release clean_debug