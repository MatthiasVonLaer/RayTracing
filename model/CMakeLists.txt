cmake_minimum_required(VERSION 3.1)

PROJECT(RaytracingModel)

INCLUDE_DIRECTORIES(
  detail
  detail/shapes/
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER mpic++)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

add_library(raytracing_model
  detail/camera.cc
  detail/color.cc
  detail/complex_light_beam.cc
  detail/controller.cc
  detail/display.cc
  detail/filter.cc
  detail/gui.cc
  detail/light_beam.cc
  detail/lens.cc
  detail/math_operation.cc
  detail/math_expression.cc
  detail/matrix.cc
  detail/mpi_manager.cc
  detail/plane.cc
  detail/polygon.cc
  detail/ray.cc
  detail/ray_diagram.cc
  detail/root_finder.cc
  detail/shapes/ball.cc
  detail/shapes/composition.cc
  detail/shapes/cube.cc
  detail/shapes/cylinder.cc
  detail/shapes/function_plot.cc
  detail/shapes/light_source.cc
  detail/shapes/plane_shape.cc
  detail/shapes/shape.cc
  detail/scene.cc
  detail/slave.cc
  detail/utilities.cc
  detail/vector.cc
)
qt5_use_modules(raytracing_model Widgets)
set_property(TARGET raytracing_model PROPERTY CXX_STANDARD 17)
