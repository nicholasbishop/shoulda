#include <sys/stat.h>

#include "libshoulda/util.hh"

namespace shoulda {

bool is_directory(const std::string& path) {
  struct stat sb;
  return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
}

std::string to_string(const CXString input) {
  const std::string output = clang_getCString(input);
  clang_disposeString(input);
  return output;
}

}

std::ostream &operator<<(std::ostream &stream, const CXString &str) {
  stream << shoulda::to_string(str);
  return stream;
}
