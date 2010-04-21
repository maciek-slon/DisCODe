## Constraints ##################################################################################
# Set FraDIA home directory.
HOMEDIR = ..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Core objects.
BASE_OBJ = 

# Binary. 
BIN = Base.so
	
## Creation scripts #############################################################################
.PHONY: all clean install depend

# Create dynamic library.
all: $(BIN)

Base.so: $(BASE_OBJ)
	$(CXX) $^ $(LDFLAGS) -shared -o $@

#	$(AR) $(ARFLAGS) $@ $?

# Remove objects/binaries/dependencies.
clean: 
	-@rm -f *.o $(BIN) .deps/*
