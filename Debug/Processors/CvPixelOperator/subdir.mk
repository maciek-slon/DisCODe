################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/CvPixelOperator/CvPixelOperator_Processor.cpp 

OBJS += \
./Processors/CvPixelOperator/CvPixelOperator_Processor.o 

CPP_DEPS += \
./Processors/CvPixelOperator/CvPixelOperator_Processor.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/CvPixelOperator/%.o: ../Processors/CvPixelOperator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


