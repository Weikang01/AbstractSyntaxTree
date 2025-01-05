cmake_minimum_required(VERSION 3.10)
project(AbstractSyntaxTree VERSION 0.0.1 LANGUAGES CXX)

file(GLOB_RECURSE _srcs src/*.cpp)

add_library(AbstractSyntaxTree STATIC ${_srcs})

set_target_properties(AbstractSyntaxTree PROPERTIES
  CXX_STANDARD 17
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN 1
)

# Unified output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_include_directories(AbstractSyntaxTree
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include # Headers in source directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src # Source files in source directory
)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

# Define UNIT_TEST for test builds
target_compile_definitions(AbstractSyntaxTree PUBLIC $<$<CONFIG:Debug>:UNIT_TEST>)