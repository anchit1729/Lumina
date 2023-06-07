cmake_minimum_required(VERSION 3.24)
project(Lumina)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lumina
        Vector3.cpp
        Vector3.h Lumina.h main.cpp Ray.h)
