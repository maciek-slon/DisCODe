## Constraints ##################################################################################
HOMEDIR = ..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Subdirectories. 
SOURCE_DIRS = Image

### Creation scripts #############################################################################
.PHONY: all clean install $(SOURCE_DIRS)

all: $(BIN) $(SOURCE_DIRS)

$(SOURCE_DIRS): $(BIN)
	@$(MAKE) -C $@

clean:
	-@rm -f *.o .deps/*
	@for i in ${SOURCE_DIRS} ; do \
		($(MAKE) -C $$i clean) ; \
	done

install:
	@for i in ${SOURCE_DIRS} ; do \
		($(MAKE) -C $$i install) ; \
	done
