cmake_minimum_required(VERSION 2.6)

project(sinoparserd)
include_directories(
    /usr/lib/i386-linux-gnu/glib-2.0/include/
    /usr/include/glibmm-2.4
    /usr/lib/glib-2.0/include/
    /usr/lib/glibmm-2.4/include
    /usr/include/glib-2.0
    /usr/lib/i386-linux-gnu/glibmm-2.4/include/
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
    glibmm-2.4
)
