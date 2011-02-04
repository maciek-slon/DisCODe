################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/MS_Sign_LUT/MS_Sign_LUT.cpp 

OBJS += \
./Processors/MS_Sign_LUT/MS_Sign_LUT.o 

CPP_DEPS += \
./Processors/MS_Sign_LUT/MS_Sign_LUT.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/MS_Sign_LUT/%.o: ../Processors/MS_Sign_LUT/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


