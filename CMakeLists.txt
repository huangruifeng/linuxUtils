cmake_minimum_required(VERSION 3.8.2)

project(utils)

set(CMAKE_OBJECT_PATH_MAX 512)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
include_directories(${SOURCE_DIRECTORY}/include)


list(APPEND source
     ${SOURCE_DIRECTORY}/include/process.h
     ${SOURCE_DIRECTORY}/source/process.cpp
    )

add_library(utils ${source})
