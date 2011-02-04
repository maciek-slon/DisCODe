################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/CvMorphology/CvMorphology_Processor.cpp 

OBJS += \
./Processors/CvMorphology/CvMorphology_Processor.o 

CPP_DEPS += \
./Processors/CvMorphology/CvMorphology_Processor.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/CvMorphology/%.o: ../Processors/CvMorphology/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


