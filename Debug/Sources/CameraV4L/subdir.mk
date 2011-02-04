################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/CameraV4L/CameraProps.cpp \
../Sources/CameraV4L/CameraV4L_Source.cpp \
../Sources/CameraV4L/PixelFormats.cpp \
../Sources/CameraV4L/PrintV4L2.cpp \
../Sources/CameraV4L/V4L.cpp \
../Sources/CameraV4L/V4L2.cpp \
../Sources/CameraV4L/VL.cpp \
../Sources/CameraV4L/VL_Common.cpp 

OBJS += \
./Sources/CameraV4L/CameraProps.o \
./Sources/CameraV4L/CameraV4L_Source.o \
./Sources/CameraV4L/PixelFormats.o \
./Sources/CameraV4L/PrintV4L2.o \
./Sources/CameraV4L/V4L.o \
./Sources/CameraV4L/V4L2.o \
./Sources/CameraV4L/VL.o \
./Sources/CameraV4L/VL_Common.o 

CPP_DEPS += \
./Sources/CameraV4L/CameraProps.d \
./Sources/CameraV4L/CameraV4L_Source.d \
./Sources/CameraV4L/PixelFormats.d \
./Sources/CameraV4L/PrintV4L2.d \
./Sources/CameraV4L/V4L.d \
./Sources/CameraV4L/V4L2.d \
./Sources/CameraV4L/VL.d \
./Sources/CameraV4L/VL_Common.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/CameraV4L/%.o: ../Sources/CameraV4L/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


