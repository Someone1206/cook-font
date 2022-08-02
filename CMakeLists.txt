cmake_minimum_required(VERSION 3.12)
project(cook-font)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCD src)
set(EXE_NAMI cook-font)
set(LIB_NAMI cook_font_lib)
set(INC_DIR inc)

file(GLOB CF_SRCS
    "${SRCD}/*.cpp"
    "${SRCD}/translator/*.cpp"
)

include_directories(AFTER ${INC_DIR})

add_library(${LIB_NAMI} STATIC ${CF_SRCS})

add_executable(${EXE_NAMI} cook-font.cpp)
target_link_libraries(${EXE_NAMI} ${LIB_NAMI})