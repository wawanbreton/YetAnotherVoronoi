cmake_minimum_required(VERSION 3.20)

option(BUILD_LIBYAV_SHARED "Build libyav as a shared library" ON)

set(libyav_SRCS
        src/yav.cpp
)

if (BUILD_LIBYAV_SHARED)
  add_library(libyav SHARED ${libyav_SRCS})
  if (WIN32)
    set_target_properties(Savitar PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
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
