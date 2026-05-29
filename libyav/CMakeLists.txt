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

set(libyav_SRCS
  src/yav.cpp

  src/generator/Generator.cpp
  src/generator/bisector/AbstractBisectorGenerator.cpp
  src/generator/bisector/EdgeEdgeBisectorGenerator.cpp
  src/generator/bisector/EdgeVertexBisectorGenerator.cpp
  src/generator/bisector/FaceEdgeBisectorGenerator.cpp
  src/generator/bisector/FaceFaceBisectorGenerator.cpp
  src/generator/bisector/FaceVertexBisectorGenerator.cpp
  src/generator/bisector/VertexVertexBisectorGenerator.cpp
  src/generator/region/AbstractVoronoiRegion.cpp
  src/generator/region/Cone.cpp
  src/generator/region/HalfPlaneSlice.cpp
  src/generator/region/Slab.cpp

  src/geometry/Point3Operations.cpp

  src/space/Space.cpp
  src/space/Primitive.cpp
  src/space/site/AbstractSite.cpp
  src/space/site/Edge.cpp
  src/space/site/Triangle.cpp
  src/space/site/Vertex.cpp

  src/voronoi/Cell.cpp
  src/voronoi/CellPatch.cpp
  src/voronoi/Diagram.cpp
  src/voronoi/equisurface/AbstractBisector.cpp
  src/voronoi/equisurface/Paraboloid.cpp
  src/voronoi/equisurface/Plane.cpp
)

if (BUILD_LIBYAV_SHARED)
  add_library(libyav SHARED ${libyav_SRCS})
  if (WIN32)
    set_target_properties(libyav PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  endif ()
else ()
  add_library(libyav STATIC ${libyav_SRCS})
endif ()

add_library(yav::libyav ALIAS libyav)

target_include_directories(libyav
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(libyav PUBLIC cxx_std_23)
target_link_libraries(libyav PUBLIC spdlog::spdlog fmt::fmt)
