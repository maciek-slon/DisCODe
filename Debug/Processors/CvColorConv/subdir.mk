################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/CvColorConv/CvColorConv_Processor.cpp 

OBJS += \
./Processors/CvColorConv/CvColorConv_Processor.o 

CPP_DEPS += \
./Processors/CvColorConv/CvColorConv_Processor.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/CvColorConv/%.o: ../Processors/CvColorConv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


