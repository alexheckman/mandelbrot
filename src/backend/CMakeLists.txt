find_package(Boost 1.49 COMPONENTS program_options REQUIRED)
find_package(Threads REQUIRED)

add_library(backend STATIC
    mandelbrot.cpp
    datasplit.cpp
    util.cpp)

target_link_libraries(backend
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )