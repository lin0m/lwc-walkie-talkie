# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}../../../../pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pioTest C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pioTest pioTest.c)
pico_set_program_name(pioTest "pioTest")
pico_set_program_version(pioTest "0.1")

pico_enable_stdio_uart(pioTest 0)
pico_enable_stdio_usb(pioTest 1)

# Add the standard library to the build
target_link_libraries(pioTest pico_stdlib)

# Add any user requested libraries
target_link_libraries(pioTest
        hardware_pio
        )

pico_add_extra_outputs(pioTest)

pico_generate_pio_header(pioTest ${CMAKE_CURRENT_LIST_DIR}/test.pio)
target_sources(pioTest PRIVATE pioTest.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)