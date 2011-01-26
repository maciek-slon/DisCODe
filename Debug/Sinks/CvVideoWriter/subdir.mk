################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sinks/CvVideoWriter/CvVideoWriter_Sink.cpp 

OBJS += \
./Sinks/CvVideoWriter/CvVideoWriter_Sink.o 

CPP_DEPS += \
./Sinks/CvVideoWriter/CvVideoWriter_Sink.d 


# Each subdirectory must supply rules for building sources it contributes
Sinks/CvVideoWriter/%.o: ../Sinks/CvVideoWriter/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


