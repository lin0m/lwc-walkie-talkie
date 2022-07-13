# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/russell/Downloads/pico-sdk")
# Lino's SDK Path:
# set(PICO_SDK_PATH "/Users/lino/pico-sdk/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(test C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
option(USE_PICO "Using pico-sdk" ON)
# Add executable. Default name is the project name, version 0.1

add_executable(test bob.c )

pico_set_program_name(test "test")
pico_set_program_version(test "0.1")

pico_enable_stdio_uart(test 1)
pico_enable_stdio_usb(test 0)

add_subdirectory(tinyJambu)
target_include_directories(test PUBLIC "${PROJECT_SOURCE_DIR}/tinyJambu")
target_link_libraries(test tinyJambu)

# Add the standard library to the build
target_link_libraries(test pico_stdlib)

# Add any user requested libraries
target_link_libraries(test
        hardware_pio
        )

pico_add_extra_outputs(test)
