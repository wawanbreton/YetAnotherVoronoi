cmake_minimum_required(VERSION 3.20)

option(BUILD_LIBYAV_SHARED "Build libyav as a shared library" ON)

include(FetchContent)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  libfmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 12.1.0
)

FetchContent_MakeAvailable(libfmt)

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  boost_geometry
  GIT_REPOSITORY https://github.com/boostorg/geometry.git
  GIT_TAG boost-1.91.0
)

FetchContent_MakeAvailable(boost_geometry)

FetchContent_Declare(
  armadillo
  GIT_REPOSITORY https://gitlab.com/conradsnicta/armadillo-code
  GIT_TAG 15.2.6
)

FetchContent_MakeAvailable(armadillo)

file(GLOB_RECURSE libyav_SRCS CONFIGURE_DEPENDS src/*.cpp)

if (BUILD_LIBYAV_SHARED)
  add_library(libyav SHARED ${libyav_SRCS}
  )
  if (WIN32)
    set_target_properties(libyav PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  endif ()
else ()
  add_library(libyav STATIC ${libyav_SRCS})
endif ()

add_library(yav::libyav ALIAS libyav)

target_include_directories(libyav PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(libyav PUBLIC cxx_std_23)
target_link_libraries(libyav PUBLIC spdlog::spdlog fmt::fmt armadillo)
