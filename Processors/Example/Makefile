## Constraints ##################################################################################
# Set FraDIA home directory.
HOMEDIR = ../..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Objects used in simple example task.
EXAMPLE_OBJ = Example_Processor.o 

# Created binaries.
PROCESSORS_SO = Example_Processor.so 

## Creation scripts #############################################################################
.PHONY: all clean install depend

# Create shared objects.
all: $(PROCESSORS_SO)

# Create SOs.
Example_Processor.so: $(EXAMPLE_OBJ)
	$(CXX) $^ $(LDFLAGS) -shared -o $@
	${INSTALL} $@ $(BINDIR)

# Install tasks to bin directory.
install: $(PROCESSORS_SO)
	${INSTALL} ${TASKS_SO} $(BINDIR)

# Remove objects/binaries/dependencies.
clean: 
	-@rm -f *.o *.so .deps/*
