cmake_minimum_required(VERSION 2.8)

PROJECT(Raytracing)

INCLUDE_DIRECTORIES(
.
shapes/
)

SET(LOCAL_SRC
camera.cc
color.cc
controller.cc
display.cc
filter.cc
light_beam.cc
lens.cc
main.cc
math_operation.cc
math_expression.cc
matrix.cc
mpi_manager.cc
polygon.cc
ray.cc
ray_diagram.cc
root_finder.cc
shapes/ball.cc
shapes/cube.cc
shapes/composition.cc
shapes/function_plot.cc
shapes/light_source.cc
shapes/plane_shape.cc
shapes/shape.cc
scene.cc
slave.cc
utilities.cc
vector.cc
)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_COMPILER mpic++)

SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Widgets)

ADD_EXECUTABLE(raytracing ${LOCAL_SRC})
QT5_USE_MODULES(raytracing Widgets)
