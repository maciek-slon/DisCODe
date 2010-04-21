## Constraints ##################################################################################
# Set FraDIA home directory.
HOMEDIR = ../..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Objects used in simple example task.
OBJ = Image_Source.o Image_Panel.o 

# Created binaries.
SOURCE = Source_Image.so 

## Creation scripts #############################################################################
.PHONY: all clean install depend

# Create shared objects.
all: $(SOURCE)

# Create SO.
Source_Image.so: $(OBJ)
	$(CXX) $^ $(LDFLAGS) -shared -o $@
	${INSTALL} $@ $(BINDIR)

# Install tasks to bin directory.
install: $(SOURCE_SO)
	${INSTALL} ${SOURCE} $(BINDIR)

# Remove objects/binaries/dependencies.
clean: 
	-@rm -f *.o *.so .deps/*
