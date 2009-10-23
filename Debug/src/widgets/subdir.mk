################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/widgets/ColorCommandWidget.cpp \
../src/widgets/Console.cpp \
../src/widgets/Fl_ImageButton.cpp \
../src/widgets/MaterialWidget.cpp \
../src/widgets/Point3DWidget.cpp \
../src/widgets/QuickLabel.cpp \
../src/widgets/RGBAWidget.cpp \
../src/widgets/TexcoordWidget.cpp \
../src/widgets/TransformWidget.cpp 

CPP_DEPS += \
./src/widgets/ColorCommandWidget.d \
./src/widgets/Console.d \
./src/widgets/Fl_ImageButton.d \
./src/widgets/MaterialWidget.d \
./src/widgets/Point3DWidget.d \
./src/widgets/QuickLabel.d \
./src/widgets/RGBAWidget.d \
./src/widgets/TexcoordWidget.d \
./src/widgets/TransformWidget.d 

OBJS += \
./src/widgets/ColorCommandWidget.o \
./src/widgets/Console.o \
./src/widgets/Fl_ImageButton.o \
./src/widgets/MaterialWidget.o \
./src/widgets/Point3DWidget.o \
./src/widgets/QuickLabel.o \
./src/widgets/RGBAWidget.o \
./src/widgets/TexcoordWidget.o \
./src/widgets/TransformWidget.o 


# Each subdirectory must supply rules for building sources it contributes
src/widgets/%.o: ../src/widgets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


