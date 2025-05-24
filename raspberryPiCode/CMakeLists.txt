# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(mqttClient C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
file(GLOB INCLUDES_FILES "includes/*.h")
file(GLOB SOURCES_FILES "src/*.c")
add_executable(mqttClient main.c 
    ${INCLUDES_FILES}
    ${SOURCES_FILES}
)

pico_set_program_name(mqttClient "mqttClient")
pico_set_program_version(mqttClient "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(mqttClient 0)
pico_enable_stdio_usb(mqttClient 1)

# Define the host name of the MQTT server in an environment variable or pass it to cmake,
# e.g. cmake -DMQTT_SERVER=myserver ..

if (DEFINED ENV{MQTT_SERVER} AND (NOT MQTT_SERVER))
    set(MQTT_SERVER $ENV{MQTT_SERVER})
    message("Using MQTT_SERVER from environment ('${MQTT_SERVER}')")
endif()
if (NOT MQTT_SERVER)
    message("Skipping MQTT example as MQTT_SERVER is not defined")
    return()
endif()
# Define the name of an MQTT broker/server to enable this example
set(MQTT_SERVER "${MQTT_SERVER}" CACHE INTERNAL "MQTT server for examples")

if (DEFINED ENV{MQTT_USERNAME} AND (NOT MQTT_USERNAME))
    set(MQTT_USERNAME $ENV{MQTT_USERNAME})
    message("Using MQTT_USERNAME from environment ('${MQTT_USERNAME}')")
endif()
set(MQTT_USERNAME "${MQTT_USERNAME}" CACHE INTERNAL "MQTT user name for examples")
if (DEFINED ENV{MQTT_PASSWORD} AND (NOT MQTT_PASSWORD))
    set(MQTT_PASSWORD $ENV{MQTT_PASSWORD})
    message("Using MQTT_PASSWORD from environment")
endif()
set(MQTT_PASSWORD "${MQTT_PASSWORD}" CACHE INTERNAL "MQTT password for examples")

# Set path to the certificate include file
if (NOT MQTT_CERT_PATH)
    set(MQTT_CERT_PATH ${CMAKE_CURRENT_LIST_DIR}/certs/${MQTT_SERVER})
endif()

# Set the name of the certificate include file
if (NOT MQTT_CERT_INC)
    set(MQTT_CERT_INC mqtt_client.inc)
endif()



# Add the standard library to the build
target_link_libraries(mqttClient
        pico_stdlib
        FreeRTOS-Kernel         # Kernel do FreeRTOS
        FreeRTOS-Kernel-Heap4   # Gerenciador de memoria
      )

# Add the standard include files to the build
target_include_directories(mqttClient PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes
        ${CMAKE_CURRENT_LIST_DIR}/..
)

# Add any user requested libraries
target_link_libraries(mqttClient 
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_adc
        hardware_i2c
        pico_multicore
        pico_lwip_mqtt
        pico_mbedtls
        pico_lwip_mbedtls
        hardware_pwm
        )

target_compile_definitions(mqttClient PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        MQTT_SERVER=\"${MQTT_SERVER}\"
        )
    if (EXISTS "${MQTT_CERT_PATH}/${MQTT_CERT_INC}")
        target_compile_definitions(mqttClient PRIVATE
            MQTT_CERT_INC=\"${MQTT_CERT_INC}\" # contains the tls certificates for MQTT_SERVER needed by the client
            ALTCP_MBEDTLS_AUTHMODE=MBEDTLS_SSL_VERIFY_REQUIRED
            )
        target_include_directories(mqttClient PRIVATE
            ${MQTT_CERT_PATH}
            )
    endif()
    if (MQTT_USERNAME AND MQTT_PASSWORD)
        target_compile_definitions(mqttClient PRIVATE
            MQTT_USERNAME=\"${MQTT_USERNAME}\"
            MQTT_PASSWORD=\"${MQTT_PASSWORD}\"
        )
    endif()
    pico_add_extra_outputs(mqttClient)
    
    # Ignore warnings from lwip code
    set_source_files_properties(
        ${PICO_LWIP_PATH}/src/apps/altcp_tls/altcp_tls_mbedtls.c
        PROPERTIES
        COMPILE_OPTIONS "-Wno-unused-result"
        )

pico_add_extra_outputs(mqttClient)

