#include "libshoulda/location.hh"
#include "libshoulda/util.hh"

namespace shoulda {

Location Location::create(const CXSourceLocation location) {
  CXFile file;
  unsigned line;
  unsigned column;
  unsigned offset;

  clang_getSpellingLocation(location,
                            &file,
                            &line,
                            &column,
                            &offset);

  return Location(to_string(clang_getFileName(file)), line, column);
}

Location::Location(
    const std::string path,
    const size_t line,
    const size_t column)
    : path_(path), line_(line), column_(column) {
}

const std::string& Location::path() const {
  return path_;
}

size_t Location::line() const {
  return line_;
}

size_t Location::column() const {
  return column_;
}

}
