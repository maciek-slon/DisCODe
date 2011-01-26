################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Types/Blobs/Blob.cpp \
../Types/Blobs/BlobContour.cpp \
../Types/Blobs/BlobOperators.cpp \
../Types/Blobs/BlobResult.cpp 

OBJS += \
./Types/Blobs/Blob.o \
./Types/Blobs/BlobContour.o \
./Types/Blobs/BlobOperators.o \
./Types/Blobs/BlobResult.o 

CPP_DEPS += \
./Types/Blobs/Blob.d \
./Types/Blobs/BlobContour.d \
./Types/Blobs/BlobOperators.d \
./Types/Blobs/BlobResult.d 


# Each subdirectory must supply rules for building sources it contributes
Types/Blobs/%.o: ../Types/Blobs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


