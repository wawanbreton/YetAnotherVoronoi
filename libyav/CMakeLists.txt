add_library(libyav src/yav.cpp)
add_library(yav::libyav ALIAS libyav)

target_include_directories(libyav
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(libyav PUBLIC cxx_std_17)
