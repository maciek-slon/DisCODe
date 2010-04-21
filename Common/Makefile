## Constraints ##################################################################################
# Set FraDIA home directory.
HOMEDIR = ..

# Include common makefile code.
include $(HOMEDIR)/common.mk

COMMON_OBJ = Panel_Empty.o
 
# Created dynamic library.
BIN = Common.so 
		
## Creation scripts #############################################################################
.PHONY: all clean install depend

# Create library and empty taskpanel.
all: $(BIN)

Common.so: $(COMMON_OBJ) 
	$(CXX) $^ $(LDFLAGS) -shared -o $@

# Remove objects/binaries/dependencies.
clean: 
	-@rm -f *.o *.so .deps/*
