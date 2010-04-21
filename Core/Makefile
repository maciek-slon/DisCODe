## Constraints ##################################################################################
# Set FraDIA home directory.
HOMEDIR = ..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Core objects.
CORE_OBJ = main.o Configurator.o
	 

# Binary. 
BIN = fradia #test_input 
	
## Creation scripts #############################################################################
.PHONY: all clean install depend

# Create FraDIA binary.
all: $(BIN)
	
fradia: $(CORE_OBJ) $(COMMONLIB) $(BASELIB)
	@echo $(PKG_CONFIG_PATH)
	$(CXX) $(LDFLAGS) -o $@ $^ 
	${INSTALL} $@ $(BINDIR)

# Install FraDIA application to bin directory.
install: $(BIN)
	@for i in ${SUBDIRS} ; do \
		($(MAKE) -C $$i install) ; \
	done
	${INSTALL} ${BIN} $(BINDIR)
	
# Remove objects/binaries/dependencies.
clean: 
	-@rm -f *.o *.so ${BIN} .deps/*
	@for i in ${SUBDIRS} ; do \
		($(MAKE) -C $$i clean) ; \
	done
