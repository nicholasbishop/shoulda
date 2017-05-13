#ifndef CURSOR_HH
#define CURSOR_HH

#include <clang-c/Index.h>

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

  CXCursorKind kind() const;

  CXType type() const;

  // TODO, wrap?
  CXSourceLocation location() const;

 private:
  const CXCursor cursor_;
};

}


#endif
