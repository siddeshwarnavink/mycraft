.PHONY: all linux wasm clean

all: linux

linux:
	$(MAKE) -f Makefile.linux

wasm:
	$(MAKE) -f Makefile.wasm

clean:
	$(MAKE) -f Makefile.linux clean
	$(MAKE) -f Makefile.wasm clean