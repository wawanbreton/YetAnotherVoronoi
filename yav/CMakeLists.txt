include(ExternalProject)

set(CXXOPTS_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/cxxopts")
file(MAKE_DIRECTORY "${CXXOPTS_INSTALL_DIR}/include")

ExternalProject_Add(cxxopts_external
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.2.0
  UPDATE_DISCONNECTED 1
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CXXOPTS_INSTALL_DIR}
    -DCXXOPTS_BUILD_EXAMPLES=OFF
    -DCXXOPTS_BUILD_TESTS=OFF
    -DCXXOPTS_BUILD_DOCS=OFF
)

add_library(cxxopts::cxxopts INTERFACE IMPORTED GLOBAL)
set_target_properties(cxxopts::cxxopts PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${CXXOPTS_INSTALL_DIR}/include"
)
add_dependencies(cxxopts::cxxopts cxxopts_external)

add_executable(yav src/main.cpp)
target_link_libraries(yav PRIVATE yav::libyav cxxopts::cxxopts)
target_compile_features(yav PRIVATE cxx_std_17)
