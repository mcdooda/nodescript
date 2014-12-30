cmake_minimum_required(VERSION 2.6)

project(nodescript)

file(
    GLOB_RECURSE
    src
    src/*
)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Werror")

add_executable(
    nodescript
    ${src}
)



