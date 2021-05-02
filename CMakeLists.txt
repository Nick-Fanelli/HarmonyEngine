cmake_minimum_required(VERSION 3.10)

project(HarmonyEngine)

file(GLOB_RECURSE SRC_FILES
        "src/**.h"
        "src/**.cpp"
)

add_executable(HarmonEngine ${SRC_FILES})