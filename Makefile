## Constraints ##################################################################################
HOMEDIR = .

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Subdirectories. 
SOURCE_DIRS = Base Common Core Sources Processors 

## Creation scripts #############################################################################
.PHONY: all clean install $(SOURCE_DIRS)

all: $(SOURCE_DIRS) 
	
Base: 
	@$(MAKE) -C $@

Common: Base
	@$(MAKE) -C $@

Core: Base Common
	@$(MAKE) -C $@

Sources: Base Common
	@$(MAKE) -C $@

Processors: Base Common
	@$(MAKE) -C $@

	
clean:
	-@rm -f $(BINDIR)/*.so 
	-@rm -f $(BINDIR)/fradia
	-@rm -f $(BINDIR)/test_input
	@for i in ${SOURCE_DIRS} ; do \
		($(MAKE) -C $$i clean) ; \
	done

install:
	@for i in ${SOURCE_DIRS} ; do \
		($(MAKE) -C $$i install) ; \
	done
