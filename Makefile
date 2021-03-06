# GNU Make workspace makefile autogenerated by Premake

.NOTPARALLEL:

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  20PinBowling_config = debug
endif
ifeq ($(config),release)
  20PinBowling_config = release
endif

PROJECTS := 20PinBowling

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

20PinBowling:
ifneq (,$(20PinBowling_config))
	@echo "==== Building 20PinBowling ($(20PinBowling_config)) ===="
	@${MAKE} --no-print-directory -C . -f 20PinBowling.make config=$(20PinBowling_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f 20PinBowling.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   20PinBowling"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"