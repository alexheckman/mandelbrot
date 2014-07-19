cmake_minimum_required (VERSION 2.6)
project(mandelbrot)
set (mandelbrot_VERSION_MAJOR 0)
set (mandelbrot_VERSION_MINOR 1)
#enable cpp11
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

configure_file(
"${mandelbrot_SOURCE_DIR}/version.h.in"
"${mandelbrot_BINARY_DIR}/version.h"
)

include_directories("${mandelbrot_BINARY_DIR}")
add_subdirectory(src)

