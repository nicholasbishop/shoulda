#include "libshoulda/util.hh"

namespace shoulda {

std::string to_string(const CXString input) {
  const std::string output = clang_getCString(input);
  clang_disposeString(input);
  return output;
}

}
