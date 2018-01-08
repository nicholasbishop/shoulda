#ifndef CURSOR_HH
#define CURSOR_HH

#include <vector>

#include <clang-c/Index.h>

#include "libshoulda/location.hh"

namespace shoulda {

class Cursor {
 public:
  Cursor(const CXCursor cursor);

  template<typename Visitor>
  void visit_children(Visitor visitor) const {
    CXClientData client_data = static_cast<CXClientData>(&visitor);
    clang_visitChildren(
        cursor_,
        [](CXCursor current, CXCursor parent, CXClientData client_data) {
          Visitor* revisitor = static_cast<Visitor*>(client_data);
          (*revisitor)(Cursor(current), Cursor(parent));
          return CXChildVisit_Recurse;
        },
        client_data);
  }

  template<typename Visitor>
  void visit_unused_return_values(Visitor visitor) const {
    visit_children(
        [&visitor](const Cursor& current, const Cursor& parent) {
          if (is_unused_return_value(current, parent)) {
            visitor(current, parent);
          }
        });
  }

  std::vector<Location> find_unused_return_values() const;

  CXCursorKind kind() const;

  Location location() const;

  CXType type() const;

  std::vector<Cursor> get_children() const;

  bool operator==(const Cursor& other) const;

 private:
  static bool is_unused_return_value(
      const Cursor& current, const Cursor& parent);

  const CXCursor cursor_;
};

}


#endif
