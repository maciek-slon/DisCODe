################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/Stereo/Stereo_Processor.cpp 

OBJS += \
./Processors/Stereo/Stereo_Processor.o 

CPP_DEPS += \
./Processors/Stereo/Stereo_Processor.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/Stereo/%.o: ../Processors/Stereo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


