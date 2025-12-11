cmake_minimum_required(VERSION 3.23)

project(crabby)

add_library(crabby INTERFACE)

target_include_directories(crabby INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/crabby DESTINATION include)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/crabbyConfigVersion.cmake"
  VERSION 0.1
  COMPATIBILITY AnyNewerVersion
)

configure_file(
  crabbyConfig.cmake.in
  crabbyConfig.cmake
  @ONLY
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/crabbyConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/crabbyConfigVersion.cmake
  DESTINATION lib/cmake/crabby
)

install(TARGETS crabby
  EXPORT crabbyTargets
)

install(EXPORT crabbyTargets
  FILE crabbyTargets.cmake
  NAMESPACE crabby::
  DESTINATION lib/cmake/crabby
)

add_executable(demo src/main.c)
target_link_libraries(demo PRIVATE crabby)
