all: release debug

run:
	cd bin/release && $(MAKE) run

run_debug:
	cd bin/debug && $(MAKE) run

release:
	cd bin/release && $(MAKE) all

debug:
	cd bin/debug && $(MAKE) all


.PHONY: clean
clean:
	cd bin/release && $(MAKE) clean
	cd bin/debug && $(MAKE) clean