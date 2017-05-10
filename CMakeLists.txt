cmake_minimum_required(VERSION 3.6)

add_executable(shoulda shoulda.cc)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# TODO
target_link_libraries(shoulda clang)