cmake_minimum_required(VERSION 3.26.4)
project(imageAnalysis1)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        imageAnalysis1
#        main.cpp
        bmp/bmp.cpp
        bmp/bmp.hpp
        bmpAnalysis/bmpAnalysis.cpp
        bmpAnalysis/bmpAnalysis.hpp
        bmpAnalysis/izo1.cpp
)
