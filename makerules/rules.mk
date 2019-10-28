# Generic makerule for project. This automate copying / symlink
# the platformio.ini file, plus having shorthand make commands

SHELL   := /bin/bash
MAKEDIR ?= ./makerules
PIOFLAG ?=

.PHONY: all compile clean upload debug
all: compile
compile clean upload debug: platformio.ini

compile:
	@pio run $(PIOFLAG)

clean:
	@pio run $(PIOFLAG) --target clean

upload:
	@pio run $(PIOFLAG) --target upload

debug:
	@pio debug $(PIOFLAG) --interface=gdb -x .pioinit

platformio.ini:
	@cp $(MAKEDIR)/platformio.ini .
