cmake_minimum_required(VERSION 2.6)

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
