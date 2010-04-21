## Constraints ##################################################################################
HOMEDIR = ..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Subdirectories. 

SOURCE_DIRS = Example


### Creation scripts #############################################################################
.PHONY: all clean install $(SOURCE_DIRS)

all: $(SOURCE_DIRS)

$(SOURCE_DIRS):
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
