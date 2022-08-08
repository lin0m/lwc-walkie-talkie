# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}../../../../pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(server C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(server server.c )

pico_set_program_name(server "server")
pico_set_program_version(server "0.1")

pico_enable_stdio_uart(server 0)
pico_enable_stdio_usb(server 1)

# Add the standard library to the build
target_link_libraries(server pico_stdlib)

pico_add_extra_outputs(server)

add_subdirectory(../include/espHelper/src espHelper)
target_link_libraries(server espHelper)
target_link_libraries(espHelper pico_stdlib)
target_include_directories(espHelper PUBLIC "../include/espHelper/src")

