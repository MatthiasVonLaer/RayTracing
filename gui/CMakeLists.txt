cmake_minimum_required(VERSION 3.1)

INCLUDE_DIRECTORIES(
  ../model/detail
  ../model/detail/shapes/
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_executable(raytracing_gui main.cc)
target_link_libraries(raytracing_gui raytracing_model)
set_property(TARGET raytracing_gui PROPERTY CXX_STANDARD 17)
