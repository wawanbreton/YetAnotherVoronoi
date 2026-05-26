cmake_minimum_required(VERSION 3.20)

include(ExternalProject)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(yavui src/main.cpp)
target_link_libraries(yavui PRIVATE yav::libyav Qt6::Widgets)
target_compile_features(yavui PRIVATE cxx_std_17)

add_dependencies(yavui libyav)
