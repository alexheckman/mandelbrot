add_library(libmandelbrot STATIC
    mandelbrot.cpp)

add_executable(mandelbrot 
    main.cpp
    util.cpp)

target_link_libraries(mandelbrot 
    libmandelbrot)
