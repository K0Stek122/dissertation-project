cmake_minimum_required(VERSION 3.10.0)
project(ids-dissertation VERSION 0.1.0 LANGUAGES C CXX)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB SOURCES "src/*.cpp" "src/*.h")
add_executable(ids-dissertation ${SOURCES})