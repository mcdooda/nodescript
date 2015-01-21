cmake_minimum_required(VERSION 2.6)

project(nodescript)

file(
    GLOB_RECURSE
    src
    src/*
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNODESCRIPT_DEBUG -DNODESCRIPT_VERBOSE")

add_executable(
    nodescript
    ${src}
)



