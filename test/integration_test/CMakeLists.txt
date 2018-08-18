cmake_minimum_required(VERSION 3.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_COMPILER mpic++)
find_package(Qt5Widgets)
find_package(Boost 1.61.0 COMPONENTS unit_test_framework)

include_directories(../../src)

add_executable(raytracing_integration_test
    main.cc
)
qt5_use_modules(raytracing_integration_test Widgets)
set_property(TARGET raytracing_integration_test PROPERTY CXX_STANDARD 17)
target_link_libraries(raytracing_integration_test
  raytracing
  stdc++fs
  ${Boost_LIBRARIES}
)

add_test(raytracing_integration_test
    raytracing_integration_test ${CMAKE_CURRENT_SOURCE_DIR}
)
