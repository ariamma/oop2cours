cmake_minimum_required(VERSION 3.5)
project(lab0)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab0 main.cpp counter.h)