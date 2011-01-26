################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/CameraUniCap/CameraUniCap_Source.cpp 

OBJS += \
./Sources/CameraUniCap/CameraUniCap_Source.o 

CPP_DEPS += \
./Sources/CameraUniCap/CameraUniCap_Source.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/CameraUniCap/%.o: ../Sources/CameraUniCap/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


