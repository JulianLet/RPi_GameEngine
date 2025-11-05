# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

# Detect user home
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(RPi_GameEngine C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------

add_subdirectory(lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)

# -----------------------------
# Gather all source files in src/
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Add main executable
add_executable(RPi_GameEngine 
    RPi_GameEngine.cpp
    ${SRC_FILES}
)

# Program name and version
pico_set_program_name(RPi_GameEngine "RPi_GameEngine")
pico_set_program_version(RPi_GameEngine "0.1")

# Generate PIO header if needed
pico_generate_pio_header(RPi_GameEngine ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Enable stdio over UART/USB
pico_enable_stdio_uart(RPi_GameEngine 0)
pico_enable_stdio_usb(RPi_GameEngine 1)

target_sources(RPi_GameEngine PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lib/glue/hw_config.c
)

# Include directories
target_include_directories(RPi_GameEngine PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src

)

# Link libraries
target_link_libraries(RPi_GameEngine
    pico_stdlib
    hardware_spi
    hardware_pio
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
)

# Extra outputs (UF2, bin, etc)
pico_add_extra_outputs(RPi_GameEngine)