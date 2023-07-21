cmake_minimum_required(VERSION 3.26.4)

project(PROJECT VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS off)
find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
)