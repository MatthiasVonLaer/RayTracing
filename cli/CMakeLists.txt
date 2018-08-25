cmake_minimum_required(VERSION 3.1)

INCLUDE_DIRECTORIES(
  ../model/detail
  ../model/detail/shapes/
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

add_executable(raytracing_cli main.cc)
target_link_libraries(raytracing_cli raytracing_model)
set_property(TARGET raytracing_cli PROPERTY CXX_STANDARD 17)
