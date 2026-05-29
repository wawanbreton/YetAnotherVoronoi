cmake_minimum_required(VERSION 3.20)

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


add_executable(yav src/main.cpp)
target_link_libraries(yav PRIVATE yav::libyav cxxopts::cxxopts rapidobj)
target_compile_features(yav PRIVATE cxx_std_17)
