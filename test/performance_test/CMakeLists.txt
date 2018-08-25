cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_COMPILER mpic++)

include_directories(../../model/detail)

add_executable(raytracing_performance_test main.cc)
target_link_libraries(raytracing_performance_test
  raytracing_model
  stdc++fs
)
set_property(TARGET raytracing_performance_test PROPERTY CXX_STANDARD 17)

add_test(raytracing_performance_test
    raytracing_performance_test ${CMAKE_CURRENT_SOURCE_DIR}
)
