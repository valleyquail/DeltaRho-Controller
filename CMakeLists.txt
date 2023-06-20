#set these as the cmake environment variables
#Roughly assumes a similar project structure
#FREERTOS_COREMQTT_MODULE=/home/<user>/CLionProjects/DeltaRho_Controller/libraries/coreMQTT-Agent;
#FREERTOS_KERNEL_PATH=/home/<user>/CLionProjects/DeltaRho_Controller/libraries/FreeRTOS/Source;
#PICO_SDK_PATH=/home/<user>/Documents/pico/pico-sdk

cmake_minimum_required(VERSION 3.13)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


include(pico_sdk_import.cmake)

include(FreeRTOS_Kernel_import.cmake)
include(FreeRTOS_coreMQTT-Agent.cmake)

project(DeltaRho_Controller C CXX ASM)
pico_sdk_init()


add_executable(DeltaRho_Controller
        src/main.cpp
        lib/I2C_Control.c lib/I2C_Control.h
        lib/DC_Motor.cpp lib/DC_Motor.h
        lib/Robot.cpp lib/Robot.h
        lib/PCA9685.h
        )

pico_enable_stdio_usb(DeltaRho_Controller 1)
pico_enable_stdio_uart(DeltaRho_Controller 1)

pico_add_extra_outputs(DeltaRho_Controller)

target_link_libraries(
        DeltaRho_Controller
        pico_stdlib
        pico_cyw43_arch_none
        hardware_i2c
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        coreMQTT
)