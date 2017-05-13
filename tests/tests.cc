#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "libshoulda/compilation_database.hh"

TEST_CASE("blah") {
  REQUIRE(1 < 2);
}
