cmake_minimum_required(VERSION 3.10)

project(del)

find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

set (CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS " -Wextra -g -O3")

add_executable(del.exe ${SOURCES})

target_link_libraries(del.exe -lGL -lsfml-graphics -lsfml-window -lsfml-system)
