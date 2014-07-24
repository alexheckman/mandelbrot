find_package(Boost 1.49 COMPONENTS program_options REQUIRED)
find_package(Threads REQUIRED)

add_executable(mandelbrot 
    main.cpp
    mandelbrot.cpp
    datasplit.cpp
    util.cpp)

target_link_libraries(mandelbrot 
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )
