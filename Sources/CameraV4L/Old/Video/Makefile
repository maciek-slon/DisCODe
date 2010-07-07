## Constraints ##################################################################################
# Set FraDIA home directory.
HOMEDIR = ../..

# Include common makefile code.
include $(HOMEDIR)/common.mk

# Video objects.
VIDEO_OBJ =  PixelFormats.o PrintV4L2.o V4L.o V4L2.o VL.o 
ifeq ($(USE_OPENCL), 1)
VIDEO_OBJ += CameraLeonardo.o
endif


BIN = Video.so
		
## Creation scripts #############################################################################
.PHONY: all clean install depend

# Create library and empty taskpanel.
all: $(BIN)

Video.so: $(VIDEO_OBJ)
	$(CXX) $^ $(LDFLAGS) -shared -o $@

# Remove objects/binaries/dependencies.
clean: 
	-@rm -f *.o *.so $(BIN) .deps/*	
	