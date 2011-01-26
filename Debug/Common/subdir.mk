################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/LoggerAux.cpp \
../Common/LoggerImpl.cpp \
../Common/ScopeLogger.cpp \
../Common/Thread.cpp \
../Common/Utils.cpp 

OBJS += \
./Common/LoggerAux.o \
./Common/LoggerImpl.o \
./Common/ScopeLogger.o \
./Common/Thread.o \
./Common/Utils.o 

CPP_DEPS += \
./Common/LoggerAux.d \
./Common/LoggerImpl.d \
./Common/ScopeLogger.d \
./Common/Thread.d \
./Common/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
Common/%.o: ../Common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


