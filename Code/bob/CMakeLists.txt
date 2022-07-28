# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)
# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/russell/Downloads/pico-sdk")
# set(PICO_SDK_PATH "/Users/lino/Research/pico-sdk/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(bob C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
option(USE_PICO "Using pico-sdk" ON)
add_definitions(-DUSE_PICO)
# Add executable. Default name is the project name, version 0.1

add_executable(bob bob.c )

pico_set_program_name(bob "bob")
pico_set_program_version(bob "0.1")

pico_enable_stdio_uart(bob 1)
pico_enable_stdio_usb(bob 0)

add_subdirectory(tinyJambu)
target_include_directories(bob PUBLIC "${PROJECT_SOURCE_DIR}/tinyJambu")
target_link_libraries(bob tinyJambu)

# Add the standard library to the build
target_link_libraries(bob pico_stdlib)

# Add any user requested libraries
target_link_libraries(bob
        hardware_pio
        )

pico_add_extra_outputs(bob)

