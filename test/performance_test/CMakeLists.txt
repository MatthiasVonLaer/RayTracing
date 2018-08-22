cmake_minimum_required(VERSION 3.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_COMPILER mpic++)
find_package(Qt5Widgets)

include_directories(../../src)

add_executable(raytracing_performance_test main.cc)
qt5_use_modules(raytracing_performance_test Widgets)
set_property(TARGET raytracing_performance_test PROPERTY CXX_STANDARD 17)
target_link_libraries(raytracing_performance_test
  raytracing
  stdc++fs
)

add_test(raytracing_performance_test
    raytracing_performance_test ${CMAKE_CURRENT_SOURCE_DIR}
)
