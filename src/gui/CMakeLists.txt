# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Gui)

add_executable(mandelbrotg
    main.cpp
    mandelbrotWidget.cpp
    )

# Add Qt5 include paths and link libraries
qt5_use_modules(mandelbrotg Widgets Gui)

# Link the backend too
target_link_libraries(mandelbrotg backend)