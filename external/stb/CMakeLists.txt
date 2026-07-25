cmake_minimum_required(VERSION 3.27)
project(stb)

set(CMAKE_CXX_STANDARD 23)

include_directories(src)




add_library(
        stb
        stb_image.h
        stb_image_impl.cpp
)

set_target_properties(stb PROPERTIES LINKER_LANGUAGE CXX)