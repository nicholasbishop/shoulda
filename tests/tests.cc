#include "libshoulda/code_index.hh"

#include "tests/w.hh"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using shoulda::CodeIndex;
using shoulda::Location;
using shoulda::UnusedReturnValue;
using shoulda::W;

// Assumes that the test program is run from a build directory inside
// the repository checkout
#define DATA(path_) ("../tests/data/" path_)

namespace {

std::vector<W> to_w(const std::vector<UnusedReturnValue> &input) {
  std::vector<W> output;
  output.reserve(input.size());
  for (const auto& elem : input) {
    output.emplace_back(W(elem.location.line(), elem.location.column()));
  }
  return output;
}

std::vector<W> load(const std::string path) {
  // TODO: assumes that the test program is run from a build directory
  // inside the repository checkout
  const std::string base_path = "../tests/data/";

  CodeIndex index;
  const auto unit = index.translation_unit_from_source_file(
      base_path + path);
  return to_w(unit.find_unused_return_values());
}

}

TEST_CASE("simple_unused_return") {
  const auto w = load("simple_unused_return.c");
  REQUIRE(w[0] == W(6, 3));
}

TEST_CASE("simple_used_return") {
  const auto w = load("simple_used_return.c");
  REQUIRE(w.empty());
}
