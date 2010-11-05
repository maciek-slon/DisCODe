################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Tools/regexfind.cpp \
../Tools/xmltree.cpp 

OBJS += \
./Tools/regexfind.o \
./Tools/xmltree.o 

CPP_DEPS += \
./Tools/regexfind.d \
./Tools/xmltree.d 


# Each subdirectory must supply rules for building sources it contributes
Tools/%.o: ../Tools/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


