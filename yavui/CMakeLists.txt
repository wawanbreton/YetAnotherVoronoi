cmake_minimum_required(VERSION 3.20)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include(FetchContent)

set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.2.0
)

FetchContent_MakeAvailable(cxxopts)

set(TINYOBJLOADER_COMPILATION_SHARED ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  rapidobj
  GIT_REPOSITORY https://github.com/guybrush77/rapidobj.git
  GIT_TAG v1.1
)

FetchContent_MakeAvailable(rapidobj)

qt_standard_project_setup(REQUIRES 6.7)

qt_add_executable(yavui src/main.cpp src/VoronoiGraphicsView.cpp)

target_include_directories(yavui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(yavui PRIVATE yav::libyav Qt6::Widgets cxxopts::cxxopts rapidobj)
target_compile_features(yavui PRIVATE cxx_std_23)

add_dependencies(yavui libyav)
