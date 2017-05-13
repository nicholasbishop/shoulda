#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "libshoulda/compilation_database.hh"

TEST_CASE("simple_unused_return") {
  
  REQUIRE(1 < 2);
}
