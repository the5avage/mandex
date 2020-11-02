all: release

run:
	cd bin/webasm && $(MAKE) run

release:
	cd bin/webasm && $(MAKE) all

.PHONY: clean
clean:
	cd bin/webasm && $(MAKE) clean