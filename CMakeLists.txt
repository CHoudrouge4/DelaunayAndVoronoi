cmake_minimum_required(VERSION 3.10)

project(del)

set (CMAKE_CXX_STANDARD 14)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS " -Wextra -g -O3")

add_executable(del.exe ${SOURCES})

target_link_libraries(del.exe -lsfml-graphics -lsfml-window -lsfml-system)
