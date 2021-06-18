cmake_minimum_required(VERSION 3.10)

project (glad)

file(GLOB_RECURSE GLAD_SRC_FILES
    "khrplatform.h"
    "glad.h"
    "glad.c"
)

add_library(glad ${GLAD_SRC_FILES})

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})