.PHONY: all pc ps2 clean

all: linux ps2

linux:
	$(MAKE) -f Makefile.linux

ps2:
	$(MAKE) -f Makefile.ps2

clean:
	$(MAKE) -f Makefile.linux clean
	$(MAKE) -f Makefile.ps2 clean