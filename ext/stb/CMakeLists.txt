cmake_minimum_required(VERSION 3.20)

# Library definition
add_library(STB_LIB
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cpp
)