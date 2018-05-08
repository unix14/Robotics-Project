################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LocalizationManager.cpp \
../src/Map.cpp \
../src/Movement.cpp \
../src/Particle.cpp \
../src/Robot.cpp \
../src/main.cpp 

OBJS += \
./src/LocalizationManager.o \
./src/Map.o \
./src/Movement.o \
./src/Particle.o \
./src/Robot.o \
./src/main.o 

CPP_DEPS += \
./src/LocalizationManager.d \
./src/Map.d \
./src/Movement.d \
./src/Particle.d \
./src/Robot.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


