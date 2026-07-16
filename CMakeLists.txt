cmake_minimum_required(VERSION 3.22)

project(
  cgride-engine
  VERSION 0.1.0
  DESCRIPTION "Build orchestration engine for Cgride."
  HOMEPAGE_URL "https://github.com/cgride/engine"
  LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CGRIDE_ENGINE_BUILD_TESTS "Build Cgride engine tests" ON)
option(CGRIDE_ENGINE_INSTALL "Install Cgride engine" ON)

if(NOT TARGET cgride::core)
  find_package(cgride-core CONFIG REQUIRED)
endif()

if(NOT TARGET cgride::project)
  find_package(cgride-project CONFIG REQUIRED)
endif()

if(NOT TARGET cgride::graph)
  find_package(cgride-graph CONFIG REQUIRED)
endif()

if(NOT TARGET cgride::toolchains)
  find_package(cgride-toolchains CONFIG REQUIRED)
endif()

if(NOT TARGET cgride::executor)
  find_package(cgride-executor CONFIG REQUIRED)
endif()

if(NOT TARGET cgride::cache)
  find_package(cgride-cache CONFIG REQUIRED)
endif()

add_library(cgride_engine
  src/build_options.cpp
  src/build_request.cpp
  src/build_result.cpp
  src/build_plan.cpp
  src/planner.cpp
  src/build_engine.cpp
)

add_library(cgride::engine ALIAS cgride_engine)

target_compile_features(cgride_engine
  PUBLIC
    cxx_std_23
)

target_include_directories(cgride_engine
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(cgride_engine
  PUBLIC
    cgride::core
    cgride::project
    cgride::graph
    cgride::toolchains
    cgride::executor
    cgride::cache
)

set_target_properties(cgride_engine
  PROPERTIES
    OUTPUT_NAME cgride_engine
    EXPORT_NAME engine
    CXX_EXTENSIONS OFF
)

if(CGRIDE_ENGINE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CGRIDE_ENGINE_INSTALL)
  install(
    TARGETS cgride_engine
    EXPORT cgride-engine-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT cgride-engine-targets
    NAMESPACE cgride::
    FILE cgride-engine-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-engine
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cgride-engine-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-engine-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-engine
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-engine-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-engine-config.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-engine-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-engine
  )
endif()
