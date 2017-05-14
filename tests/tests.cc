#include <algorithm>
#include <iterator>

#include "libshoulda/code_index.hh"

#include "tests/w.hh"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using shoulda::CodeIndex;
using shoulda::Location;
using shoulda::W;

// Assumes that the test program is run from a build directory inside
// the repository checkout
#define DATA(path_) ("../tests/data/" path_)

namespace {

std::vector<W> to_w(const std::vector<Location> &input) {
  std::vector<W> output;
  output.reserve(input.size());
  for (const auto& elem : input) {
    output.emplace_back(W(elem.line(), elem.column()));
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
  return to_w(unit.cursor().find_unused_return_values());
}

}

TEST_CASE("simple_unused_return") {
  const auto e = {W(6, 3)};
  REQUIRE(load("simple_unused_return.c") == e);
}

TEST_CASE("simple_used_return") {
  REQUIRE(load("simple_used_return.c").empty());
}

TEST_CASE("if_condition") {
  REQUIRE(load("if_condition.c").empty());
}

TEST_CASE("if_braced_body") {
  const auto e = {W(7, 5)};
  REQUIRE(load("if_braced_body.c") == e);
}

TEST_CASE("if_unbraced_body") {
  const auto e = {W(7, 5)};
  REQUIRE(load("if_unbraced_body.c") == e);
}
