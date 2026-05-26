cmake_minimum_required(VERSION 3.20)

project(YetAnotherVoronoi VERSION 0.1.0 LANGUAGES CXX)

option(YAV_BUILD_CLI "Build the yav command-line test program" OFF)
option(YAV_BUILD_UI "Build the yavui Qt test program" OFF)

add_subdirectory(libyav)

if(YAV_BUILD_CLI)
  add_subdirectory(yav)
endif()

if(YAV_BUILD_UI)
  add_subdirectory(yavui)
endif()
