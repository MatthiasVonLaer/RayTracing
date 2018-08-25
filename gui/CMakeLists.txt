cmake_minimum_required(VERSION 3.1)

INCLUDE_DIRECTORIES(..)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_executable(raytracing_gui
    main.cc 
    gui.cc
)
target_link_libraries(raytracing_gui
    raytracing_model
    Qt5::Widgets
)
set_property(TARGET raytracing_gui PROPERTY CXX_STANDARD 17)
