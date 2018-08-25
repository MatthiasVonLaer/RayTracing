cmake_minimum_required(VERSION 3.1)

PROJECT(RaytracingModel)

INCLUDE_DIRECTORIES(
  .
  shapes
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

find_package(Qt5Widgets)

add_library(raytracing_model
  camera.cc
  color.cc
  complex_light_beam.cc
  controller.cc
  display.cc
  filter.cc
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
qt5_use_modules(raytracing_model Widgets)
set_property(TARGET raytracing_model PROPERTY CXX_STANDARD 17)
