cmake_minimum_required(VERSION 3.1)

PROJECT(Raytracing)

enable_testing()

INCLUDE_DIRECTORIES(
.
shapes/
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_library(raytracing
  camera.cc
  color.cc
  complex_light_beam.cc
  controller.cc
  display.cc
  filter.cc
  gui.cc
  light_beam.cc
  lens.cc
  math_operation.cc
  math_expression.cc
  matrix.cc
  mpi_manager.cc
  plane.cc
  polygon.cc
  ray.cc
  ray_diagram.cc
  root_finder.cc
  shapes/ball.cc
  shapes/composition.cc
  shapes/cube.cc
  shapes/cylinder.cc
  shapes/function_plot.cc
  shapes/light_source.cc
  shapes/plane_shape.cc
  shapes/shape.cc
  scene.cc
  slave.cc
  utilities.cc
  vector.cc
)
qt5_use_modules(raytracing Widgets)
set_property(TARGET raytracing PROPERTY CXX_STANDARD 17)

add_executable(raytracing_cli main.cc)
target_link_libraries(raytracing_cli raytracing)
set_property(TARGET raytracing_cli PROPERTY CXX_STANDARD 17)
