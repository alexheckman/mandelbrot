cmake_minimum_required (VERSION 2.8)
project(mandelbrot)
set (mandelbrot_VERSION_MAJOR 0)
set (mandelbrot_VERSION_MINOR 1)

#enable cpp11
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

#workaround for bug on 13.10
#https://bugs.launchpad.net/ubuntu/+source/gcc-defaults/+bug/1228201
if (UNIX)
    set(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}")
endif()


configure_file(
"${mandelbrot_SOURCE_DIR}/version.h.in"
"${mandelbrot_BINARY_DIR}/version.h"
)

include_directories("${mandelbrot_BINARY_DIR}")
add_subdirectory(src)

