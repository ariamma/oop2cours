cmake_minimum_required(VERSION 3.26)
project(lab2)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab2 main.cpp Cell.h Cell.cpp Rule.h Rule.cpp Universe.h Universe.cpp Game.h Game.cpp)