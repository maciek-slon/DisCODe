################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Tmp/LoggerTest.cpp \
../Tmp/OpenCVSpeed.cpp \
../Tmp/PolicyTest.cpp \
../Tmp/TestOpenCV.cpp \
../Tmp/TestUnicap.cpp \
../Tmp/TestV4L.cpp \
../Tmp/ptree.cpp 

OBJS += \
./Tmp/LoggerTest.o \
./Tmp/OpenCVSpeed.o \
./Tmp/PolicyTest.o \
./Tmp/TestOpenCV.o \
./Tmp/TestUnicap.o \
./Tmp/TestV4L.o \
./Tmp/ptree.o 

CPP_DEPS += \
./Tmp/LoggerTest.d \
./Tmp/OpenCVSpeed.d \
./Tmp/PolicyTest.d \
./Tmp/TestOpenCV.d \
./Tmp/TestUnicap.d \
./Tmp/TestV4L.d \
./Tmp/ptree.d 


# Each subdirectory must supply rules for building sources it contributes
Tmp/%.o: ../Tmp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


