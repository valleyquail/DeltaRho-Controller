cmake_minimum_required(VERSION 3.13)

# Include filepaths for source and include.
if (DEFINED ENV{FREERTOS_COREMQTT_MODULE})
    set(FREERTOS_COREMQTT_MODULE $ENV{FREERTOS_COREMQTT_MODULE})
    message("Using FREERTOS_COREMQTT_MODULE from environment ('${FREERTOS_COREMQTT_MODULE}')")
else ()
    set(FREERTOS_COREMQTT_MODULE "${FREERTOS_KERNEL_PATH}/../../coreMQTT-Agent")
    message("Using FREERTOS_COREMQTT_MODULE from environment ('${FREERTOS_COREMQTT_MODULE}')")
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

## MQTT Agent library Public Include directories.
#set(MQTT_AGENT_INCLUDE_PUBLIC_DIRS
#        "${FREERTOS_COREMQTT_MODULE}/source/include")
#
### MQTT Agent library source files.
#set(MQTT_AGENT_SOURCES
#        "${FREERTOS_COREMQTT_MODULE}/source/core_mqtt_agent.c"
#        "${FREERTOS_COREMQTT_MODULE}/source/core_mqtt_agent_command_functions.c")
add_library(coreMQTT INTERFACE)

include(${FREERTOS_COREMQTT_MODULE}/source/dependency/coreMQTT/mqttFilePaths.cmake)
include(${FREERTOS_COREMQTT_MODULE}/mqttAgentFilePaths.cmake)

#
target_sources(coreMQTT PUBLIC
        ${MQTT_AGENT_SOURCES}
        ${MQTT_SOURCES}
        ${MQTT_SERIALIZER_SOURCES}
        )
# MQTT public include path.
target_include_directories(coreMQTT INTERFACE ${MQTT_INCLUDE_PUBLIC_DIRS})

# Build MQTT library target without custom config dependency.
target_compile_definitions(coreMQTT INTERFACE MQTT_AGENT_DO_NOT_USE_CUSTOM_CONFIG=1)


