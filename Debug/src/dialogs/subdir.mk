################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dialogs/AdvancedOptionsDialog.cpp \
../src/dialogs/ArcConfigurationDialog.cpp \
../src/dialogs/BaseConfigurationDialog.cpp \
../src/dialogs/BoxConfigurationDialog.cpp \
../src/dialogs/ConeConfigurationDialog.cpp \
../src/dialogs/ConfigurationMenu.cpp \
../src/dialogs/DefineEditor.cpp \
../src/dialogs/DynamicColorConfigurationDialog.cpp \
../src/dialogs/Fl_Dialog.cpp \
../src/dialogs/Fl_Error.cpp \
../src/dialogs/Fl_Tweak.cpp \
../src/dialogs/GroupConfigurationDialog.cpp \
../src/dialogs/InfoConfigurationDialog.cpp \
../src/dialogs/MasterConfigurationDialog.cpp \
../src/dialogs/MaterialConfigurationDialog.cpp \
../src/dialogs/MaterialEditor.cpp \
../src/dialogs/MenuBar.cpp \
../src/dialogs/PhysicsConfigurationDialog.cpp \
../src/dialogs/PhysicsEditor.cpp \
../src/dialogs/PyramidConfigurationDialog.cpp \
../src/dialogs/RenameDialog.cpp \
../src/dialogs/SnapSettings.cpp \
../src/dialogs/SphereConfigurationDialog.cpp \
../src/dialogs/TeleporterConfigurationDialog.cpp \
../src/dialogs/TextureMatrixConfigurationDialog.cpp \
../src/dialogs/WeaponConfigurationDialog.cpp \
../src/dialogs/WorldOptionsDialog.cpp \
../src/dialogs/ZoneConfigurationDialog.cpp 

CPP_DEPS += \
./src/dialogs/AdvancedOptionsDialog.d \
./src/dialogs/ArcConfigurationDialog.d \
./src/dialogs/BaseConfigurationDialog.d \
./src/dialogs/BoxConfigurationDialog.d \
./src/dialogs/ConeConfigurationDialog.d \
./src/dialogs/ConfigurationMenu.d \
./src/dialogs/DefineEditor.d \
./src/dialogs/DynamicColorConfigurationDialog.d \
./src/dialogs/Fl_Dialog.d \
./src/dialogs/Fl_Error.d \
./src/dialogs/Fl_Tweak.d \
./src/dialogs/GroupConfigurationDialog.d \
./src/dialogs/InfoConfigurationDialog.d \
./src/dialogs/MasterConfigurationDialog.d \
./src/dialogs/MaterialConfigurationDialog.d \
./src/dialogs/MaterialEditor.d \
./src/dialogs/MenuBar.d \
./src/dialogs/PhysicsConfigurationDialog.d \
./src/dialogs/PhysicsEditor.d \
./src/dialogs/PyramidConfigurationDialog.d \
./src/dialogs/RenameDialog.d \
./src/dialogs/SnapSettings.d \
./src/dialogs/SphereConfigurationDialog.d \
./src/dialogs/TeleporterConfigurationDialog.d \
./src/dialogs/TextureMatrixConfigurationDialog.d \
./src/dialogs/WeaponConfigurationDialog.d \
./src/dialogs/WorldOptionsDialog.d \
./src/dialogs/ZoneConfigurationDialog.d 

OBJS += \
./src/dialogs/AdvancedOptionsDialog.o \
./src/dialogs/ArcConfigurationDialog.o \
./src/dialogs/BaseConfigurationDialog.o \
./src/dialogs/BoxConfigurationDialog.o \
./src/dialogs/ConeConfigurationDialog.o \
./src/dialogs/ConfigurationMenu.o \
./src/dialogs/DefineEditor.o \
./src/dialogs/DynamicColorConfigurationDialog.o \
./src/dialogs/Fl_Dialog.o \
./src/dialogs/Fl_Error.o \
./src/dialogs/Fl_Tweak.o \
./src/dialogs/GroupConfigurationDialog.o \
./src/dialogs/InfoConfigurationDialog.o \
./src/dialogs/MasterConfigurationDialog.o \
./src/dialogs/MaterialConfigurationDialog.o \
./src/dialogs/MaterialEditor.o \
./src/dialogs/MenuBar.o \
./src/dialogs/PhysicsConfigurationDialog.o \
./src/dialogs/PhysicsEditor.o \
./src/dialogs/PyramidConfigurationDialog.o \
./src/dialogs/RenameDialog.o \
./src/dialogs/SnapSettings.o \
./src/dialogs/SphereConfigurationDialog.o \
./src/dialogs/TeleporterConfigurationDialog.o \
./src/dialogs/TextureMatrixConfigurationDialog.o \
./src/dialogs/WeaponConfigurationDialog.o \
./src/dialogs/WorldOptionsDialog.o \
./src/dialogs/ZoneConfigurationDialog.o 


# Each subdirectory must supply rules for building sources it contributes
src/dialogs/%.o: ../src/dialogs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I../include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


