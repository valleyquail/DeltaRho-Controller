#set these as the cmake environment variables
#Roughly assumes a similar project structure
#FREERTOS_COREMQTT_MODULE=/home/<user>/CLionProjects/DeltaRho_Controller/libraries/coreMQTT-Agent;
#FREERTOS_KERNEL_PATH=/home/<user>/CLionProjects/DeltaRho_Controller/libraries/FreeRTOS/Source;
#PICO_SDK_PATH=/home/<user>/Documents/pico/pico-sdk

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

#Includes the FreeRTOSConfig.h file
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_compile_definitions(freertos_config INTERFACE)

include(FreeRTOS_Kernel_import.cmake)


add_executable(DeltaRho_Controller
        src/main.cpp)

target_include_directories(DeltaRho_Controller SYSTEM PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/src)


pico_enable_stdio_usb(DeltaRho_Controller 1)
pico_enable_stdio_uart(DeltaRho_Controller 1)

pico_add_extra_outputs(DeltaRho_Controller)

target_link_libraries(
        DeltaRho_Controller
        freertos_config
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_cyw43_arch_lwip_sys_freertos
        pico_lwip_mqtt
        pico_stdlib
        hardware_i2c

)
