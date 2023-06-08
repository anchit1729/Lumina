cmake_minimum_required(VERSION 3.24)
project(Lumina)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lumina
        vec3.h lumina.h main.cpp ray.h hittable.h sphere.h hittable_list.h camera.h)
