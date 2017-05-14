#ifndef W_HH
#define W_HH

#include <cstdint>
#include <ostream>

namespace shoulda {

// W, short for "where". Just a convenience to make test assertions
// easier to write.
class W {
 public:
  W(const size_t line, const size_t column)
      : line(line), col(column) {
  }

  bool operator==(const W& other) const {
    return line == other.line && col == other.col;
  }

  const size_t line;
  const size_t col;
};

inline std::ostream& operator<<(std::ostream& os, const W& w) {
  os << "W(" << w.line << ", " << w.col << ")";
  return os;
}

inline bool operator==(
    const std::initializer_list<W> &inl, const std::vector<W>& vec) {
  return std::equal(std::begin(inl), std::end(inl),
                    std::begin(vec), std::end(vec));
}

inline bool operator==(
    const std::vector<W>& vec, const std::initializer_list<W> &inl) {
  return inl == vec;
}

}

#endif
