cmake_minimum_required(VERSION 3.10)

project (HarmonyEngine)

set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++20 -Wall -I/usr/local/include -I/opt/X11/include -I/opt/X11/lib -I${CMAKE_CURRENT_SOURCE_DIR}/include")
set(GCC_COVERAGE_LINK_FLAGS    "-lglfw -lGLEW -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -L/usr/local/lib")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_subdirectory(HarmonyEngine)
add_subdirectory(HarmonyEditor) 