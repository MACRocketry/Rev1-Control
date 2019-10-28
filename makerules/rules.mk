# Generic makerule for lib source file. This automate copying / symlink
# the platformio.ini file, plus having shorthand make commands


# project folder structure
PROJDIR ?= ./
MAKEDIR ?= $(PROJDIR)/makerules


# platformio environment variables
ifneq ($(SRC_DIR),)
export PLATFORMIO_SRC_DIR=$(SRC_DIR)
endif
ifneq ($(LIB_DIR),)
export PLATFORMIO_LIB_DIR=$(LIB_DIR)
endif


# platformio flags
PIOFLAG ?=

# makefile shell (/bin/sh is fine too)
SHELL   := /bin/bash


# ----- makefile targets and commands ----- #
.PHONY: all compile clean cleanall upload debug echo_env
all: compile
compile clean upload debug: platformio.ini

compile: echo_env
	@pio run $(PIOFLAG)

clean:
	pio run $(PIOFLAG) --target clean

cleanall:
	@if [[ -e platformio.ini ]]; then pio run $(PIOFLAG) --target clean; fi
	@rm -rf platformio.ini .pio

upload:
	@pio run $(PIOFLAG) --target upload

debug:
	@pio debug $(PIOFLAG) --interface=gdb -x .pioinit

platformio.ini:
	@cp $(MAKEDIR)/platformio.ini .

echo_env:
	@if [[ ! -z "$(PLATFORMIO_SRC_DIR)" ]];then \
		echo export PLATFORMIO_SRC_DIR=$(PLATFORMIO_SRC_DIR); \
	fi
	@if [[ ! -z "$(PLATFORMIO_LIB_DIR)" ]];then \
		echo export PLATFORMIO_LIB_DIR=$(PLATFORMIO_LIB_DIR); \
	fi

