################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/MS_Barcode_Macro/MS_Barcode_Macro.cpp 

OBJS += \
./Processors/MS_Barcode_Macro/MS_Barcode_Macro.o 

CPP_DEPS += \
./Processors/MS_Barcode_Macro/MS_Barcode_Macro.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/MS_Barcode_Macro/%.o: ../Processors/MS_Barcode_Macro/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


