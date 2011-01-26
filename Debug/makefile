################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include doc_src/examples/subdir.mk
-include build/CMakeFiles/CompilerIdCXX/subdir.mk
-include build/CMakeFiles/CompilerIdC/subdir.mk
-include Types/Blobs/subdir.mk
-include Tools/subdir.mk
-include Tmp/subdir.mk
-include Sources/Sequence/subdir.mk
-include Sources/Movie/subdir.mk
-include Sources/Keyboard/subdir.mk
-include Sources/Image/subdir.mk
-include Sources/CameraV4L/subdir.mk
-include Sources/CameraUniCap/subdir.mk
-include Sources/CameraOpenCV/subdir.mk
-include Sinks/CvWindow/subdir.mk
-include Sinks/CvVideoWriter/subdir.mk
-include Processors/Stereo/subdir.mk
-include Processors/MS_Sign_LUT/subdir.mk
-include Processors/MS_Sign_Decide/subdir.mk
-include Processors/MS_Barcode_Prepare/subdir.mk
-include Processors/MS_Barcode_Macro/subdir.mk
-include Processors/MS_Barcode_Decide/subdir.mk
-include Processors/ExecTimer/subdir.mk
-include Processors/Example/subdir.mk
-include Processors/CvThreshold/subdir.mk
-include Processors/CvSplit/subdir.mk
-include Processors/CvPixelOperator/subdir.mk
-include Processors/CvMorphology/subdir.mk
-include Processors/CvGaussianBlur/subdir.mk
-include Processors/CvFilter2D/subdir.mk
-include Processors/CvColorConv/subdir.mk
-include Processors/CameraCalib/subdir.mk
-include Processors/BlobExtractor/subdir.mk
-include Core/subdir.mk
-include Common/subdir.mk
-include Base/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: discode

# Tool invocations
discode: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o"discode" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) discode
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
