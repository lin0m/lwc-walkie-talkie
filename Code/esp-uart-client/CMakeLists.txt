# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/russell/Downloads/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(esp-uart-client C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(esp-uart-client esp-uart-client.c )

pico_set_program_name(esp-uart-client "esp-uart-client")
pico_set_program_version(esp-uart-client "0.1")

pico_enable_stdio_uart(esp-uart-client 0)
pico_enable_stdio_usb(esp-uart-client 1)

# Add the standard library to the build
target_link_libraries(esp-uart-client pico_stdlib)

pico_add_extra_outputs(esp-uart-client)

