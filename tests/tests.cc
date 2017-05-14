#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "libshoulda/code_index.hh"

using shoulda::CodeIndex;

// Assumes that the test program is run from a build directory inside
// the repository checkout
#define DATA(path_) ("../tests/data/" path_)

TEST_CASE("simple_unused_return") {
  CodeIndex index;
  const auto unit = index.translation_unit_from_source_file(
      DATA("simple_unused_return.c"));
  REQUIRE(1 < 2);
}
