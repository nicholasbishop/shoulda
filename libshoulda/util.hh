#ifndef UTIL_HH
#define UTIL_HH

#include <ostream>
#include <string>

#include <clang-c/Index.h>

namespace shoulda {

bool is_directory(const std::string& path);

std::string to_string(const CXString input);

}

std::ostream &operator<<(std::ostream &stream, const CXString &str);

#endif
