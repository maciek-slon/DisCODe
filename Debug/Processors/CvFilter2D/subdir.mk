################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/CvFilter2D/CvFilter2D_Processor.cpp 

OBJS += \
./Processors/CvFilter2D/CvFilter2D_Processor.o 

CPP_DEPS += \
./Processors/CvFilter2D/CvFilter2D_Processor.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/CvFilter2D/%.o: ../Processors/CvFilter2D/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


