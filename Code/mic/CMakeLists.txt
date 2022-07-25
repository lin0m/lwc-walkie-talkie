# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/russell/Downloads/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(micTest C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(micTest pioTest.c)
pico_set_program_name(micTest "micTest")
pico_set_program_version(micTest "0.1")

pico_enable_stdio_uart(micTest 1)
pico_enable_stdio_usb(micTest 1)

# Add the standard library to the build
target_link_libraries(micTest pico_stdlib)

# Add any user requested libraries
target_link_libraries(micTest
        hardware_pio
        )

pico_add_extra_outputs(micTest)

pico_generate_pio_header(micTest ${CMAKE_CURRENT_LIST_DIR}/test.pio)
target_sources(micTest PRIVATE pioTest.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)