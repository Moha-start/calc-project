cmake_minimum_required(VERSION 3.10)
project(calculator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Use debug symbols by default so the executables can be debugged in VS Code.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(CTest)
enable_testing()

add_library(Stack SHARED stack.c)
add_library(Calc SHARED calc.c)
target_link_libraries(Calc PRIVATE Stack)

add_executable(calculator main.c)
target_link_libraries(calculator PRIVATE Calc)

add_executable(stack stack_main.c)
target_link_libraries(stack PRIVATE Stack)

