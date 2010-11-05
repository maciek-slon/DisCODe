################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/Sequence/Sequence.cpp 

OBJS += \
./Sources/Sequence/Sequence.o 

CPP_DEPS += \
./Sources/Sequence/Sequence.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Sequence/%.o: ../Sources/Sequence/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


