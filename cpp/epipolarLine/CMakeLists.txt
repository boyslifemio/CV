# minimum version of CMake
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(epipolarLine)

set(CMAKE_CXX_COMPILER g++)

# Use precompiled headers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

find_package(OpenCV REQUIRED)
IF (OpenCV_FOUND)
  message([message] OpenCV_VER:${OpenCV_VERSION})
  include_directories(${OpenCV_INCLUDE_DIRS})
  link_directories(${OpenCV_LIBRARY_DIRS})
  add_definitions(${OpenCV_DEFINITIONS})
ENDIF()


add_executable(
  drawEpipolarLines.out
  epipolar_beta.hpp
  epipolar_beta.cpp
  )

target_link_libraries(
  drawEpipolarLines.out
  ${OpenCV_LIBS}
  ${PCL_LIBRARIES}
  )
