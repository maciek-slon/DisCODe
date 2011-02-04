################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/MS_Barcode_Prepare/MS_Barcode_Prepare.cpp 

OBJS += \
./Processors/MS_Barcode_Prepare/MS_Barcode_Prepare.o 

CPP_DEPS += \
./Processors/MS_Barcode_Prepare/MS_Barcode_Prepare.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/MS_Barcode_Prepare/%.o: ../Processors/MS_Barcode_Prepare/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


