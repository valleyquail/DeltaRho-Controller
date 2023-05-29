cmake_minimum_required(VERSION 3.13)
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

project(DeltaRho_Controller C CXX ASM)
pico_sdk_init()

add_executable(DeltaRho_Controller src/main.c lib/I2C.c lib/I2C.h lib/Motor_Controller.cpp lib/Motor_Controller.h)

pico_enable_stdio_usb(DeltaRho_Controller 1)
pico_enable_stdio_uart(DeltaRho_Controller 1)

pico_add_extra_outputs(DeltaRho_Controller)
target_link_libraries(DeltaRho_Controller
        pico_stdlib
        pico_cyw43_arch_none
        )