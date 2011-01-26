################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/ExecTimer/ExecTimer.cpp 

OBJS += \
./Processors/ExecTimer/ExecTimer.o 

CPP_DEPS += \
./Processors/ExecTimer/ExecTimer.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/ExecTimer/%.o: ../Processors/ExecTimer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


