cmake_minimum_required(VERSION 3.13)
project(stdEspHelper VERSION 0.1)
include(CTest)
enable_testing()

add_library(stdEspHelper src/stdEspHelper.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
add_compile_options(-Wall -Wextra)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)