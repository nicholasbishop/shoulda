cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(
  libshoulda
  libshoulda/code_index.cc
  libshoulda/compilation_database.cc
  libshoulda/cursor.cc
  libshoulda/error.cc
  libshoulda/translation_unit.cc)

add_executable(
  shoulda
  shoulda.cc)

add_executable(
  tests
  tests/tests.cc)

target_link_libraries(shoulda clang libshoulda)

target_link_libraries(tests clang libshoulda)