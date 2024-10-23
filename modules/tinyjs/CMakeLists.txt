cmake_minimum_required(VERSION 3.5.0)

project(TinyJS)

include_directories(include)
include_directories(include/tinyjs)

add_library(${PROJECT_NAME} STATIC
    TinyJS.cpp
    TinyJS_Functions.cpp
    TinyJS_MathFunctions.cpp
)