#ifndef UTIL_HH
#define UTIL_HH

#include <string>

#include <clang-c/Index.h>

namespace shoulda {

std::string to_string(const CXString input);

}

#endif
