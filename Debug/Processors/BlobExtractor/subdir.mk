################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/BlobExtractor/BlobExtractor_Processor.cpp \
../Processors/BlobExtractor/ComponentLabeling.cpp 

OBJS += \
./Processors/BlobExtractor/BlobExtractor_Processor.o \
./Processors/BlobExtractor/ComponentLabeling.o 

CPP_DEPS += \
./Processors/BlobExtractor/BlobExtractor_Processor.d \
./Processors/BlobExtractor/ComponentLabeling.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/BlobExtractor/%.o: ../Processors/BlobExtractor/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


