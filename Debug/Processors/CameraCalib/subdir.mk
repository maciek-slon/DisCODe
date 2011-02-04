################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Processors/CameraCalib/CameraCalib.cpp 

OBJS += \
./Processors/CameraCalib/CameraCalib.o 

CPP_DEPS += \
./Processors/CameraCalib/CameraCalib.d 


# Each subdirectory must supply rules for building sources it contributes
Processors/CameraCalib/%.o: ../Processors/CameraCalib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


