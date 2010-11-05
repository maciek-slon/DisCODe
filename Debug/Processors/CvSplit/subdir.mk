################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/CvSplit/CvSplit_Processor.cpp 

OBJS += \
./Processors/CvSplit/CvSplit_Processor.o 

CPP_DEPS += \
./Processors/CvSplit/CvSplit_Processor.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/CvSplit/%.o: ../Processors/CvSplit/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


