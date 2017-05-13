cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  shoulda
  shoulda.cc
  libshoulda/compilation_database.cc
  libshoulda/error.cc)

# TODO
target_link_libraries(shoulda clang)