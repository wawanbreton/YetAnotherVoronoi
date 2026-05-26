cmake_minimum_required(VERSION 3.20)

include(ExternalProject)

ExternalProject_Add(qtbase_external
  GIT_REPOSITORY https://code.qt.io/qt/qtbase.git
  GIT_TAG v6.7.2
  UPDATE_DISCONNECTED 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(yavui src/main.cpp)
target_link_libraries(yavui PRIVATE yav::libyav Qt6::Widgets)
target_compile_features(yavui PRIVATE cxx_std_17)

add_dependencies(yavui qtbase_external)
