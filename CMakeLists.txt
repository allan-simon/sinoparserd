cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(sinoparserd)
include_directories(
    src
)

file(
    GLOB
    source_files
    src/*.cpp
)

add_executable(
    sinoparserd
    ${source_files}
)

target_link_libraries(
    sinoparserd
    stdc++
    expat
    event
)
