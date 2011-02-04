################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../doc_src/examples/logger_example_1.cpp 

OBJS += \
./doc_src/examples/logger_example_1.o 

CPP_DEPS += \
./doc_src/examples/logger_example_1.d 


# Each subdirectory must supply rules for building sources it contributes
doc_src/examples/%.o: ../doc_src/examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


