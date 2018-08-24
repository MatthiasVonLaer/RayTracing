cmake_minimum_required(VERSION 3.1)

PROJECT(Raytracing)
enable_testing()
subdirs(
  model
  test
  cli
  gui
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
