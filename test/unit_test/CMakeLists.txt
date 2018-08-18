cmake_minimum_required(VERSION 3.1)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Boost 1.61.0 COMPONENTS unit_test_framework)

include_directories(../../src)

add_executable(raytracing_unit_test
    ../../src/shapes/ball_test.cc
    main.cc
)
qt5_use_modules(raytracing_unit_test Widgets)
target_link_libraries(raytracing_unit_test
  raytracing
  ${Boost_LIBRARIES}
)
add_test(raytracing_unit_test raytracing_unit_test)
