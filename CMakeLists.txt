cmake_minimum_required(VERSION 3.10.0)
project(ids-dissertation VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

file(GLOB SOURCES "src/*.cpp")
add_executable(ids-dissertation ${SOURCES})

target_include_directories(ids-dissertation PRIVATE ${CMAKE_SOURCE_DIR}/src/pcapplusplus/include)
set(PCPP_LIB_DIR "${CMAKE_SOURCE_DIR}/pcapplusplus/lib/")
find_library(PCPP_PCAP NAMES Pcap++ PATHS ${PCPP_LIB_DIR} REQUIRED)
find_library(PCPP_PACKET NAMES Packet++ PATHS ${PCPP_LIB_DIR} REQUIRED)
find_library(PCPP_COMMON NAMES Common++ PATHS ${PCPP_LIB_DIR} REQUIRED)

target_link_libraries(ids-dissertation, PRIVATE
    ${PCPP_PCAP} 
    ${PCPP_PACKET}
    ${PCPP_COMMON} 
    pcap
    pthread
)