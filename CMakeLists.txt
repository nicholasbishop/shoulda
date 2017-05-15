cmake_minimum_required(VERSION 3.0.0)

# Bumped this from 11 to 14 to expose a new std::equal signature:
#     bool equal(InputIt1 first1, InputIt1 last1,
#                InputIt2 first2, InputIt2 last2);
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# TODO needed for libclang++?
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fno-rtti")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(
  libshoulda
  libshoulda/code_index.cc
  libshoulda/compilation_database.cc
  libshoulda/cursor.cc
  libshoulda/error.cc
  libshoulda/location.cc
  libshoulda/translation_unit.cc
  libshoulda/util.cc)

add_executable(
  shoulda
  shoulda.cc)

add_executable(
  tests
  tests/tests.cc)

target_link_libraries(
  shoulda
  #libshoulda
  clang
  clangAST
  clangBasic
  clangFrontend
  clangTooling
  LLVM
  )

target_link_libraries(tests clang libshoulda)