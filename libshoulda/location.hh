#ifndef LOCATION_HH
#define LOCATION_HH

#include <cstdint>
#include <string>

#include <clang-c/Index.h>

namespace shoulda {

class Location {
 public:
  static Location create(const CXSourceLocation location);

  Location(
      const std::string path,
      const size_t line,
      const size_t column);

  const std::string& path() const;

  size_t line() const;

  size_t column() const;

  bool operator==(const Location& other) const;

 private:
  const std::string path_;
  const size_t line_;
  const size_t column_;
};

}

#endif
