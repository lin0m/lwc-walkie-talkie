cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)
set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}../../../../pico-sdk")
include(pico_sdk_import.cmake)
project(alice C CXX ASM)
add_executable(alice alice.c)
pico_sdk_init()

pico_set_program_name(alice "alice")
pico_set_program_version(alice "0.1")
pico_enable_stdio_usb(alice 0)

add_subdirectory(../include/dac dac)
target_link_libraries(dac pico_stdlib)
target_include_directories(dac PUBLIC "../include/dac/src")
add_subdirectory(../include/espHelper/src espHelper)
target_link_libraries(espHelper pico_stdlib)
target_include_directories(espHelper PUBLIC "../include/espHelper/src")
add_subdirectory(include tinyJambu)
target_include_directories(tinyJambu PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_link_libraries(alice pico_stdlib
                    hardware_pio
                    espHelper
                    dac
                    tinyJambu
)
pico_add_extra_outputs(alice)
pico_enable_stdio_usb(espHelper 1)
pico_enable_stdio_usb(dac 1)
