#include "libshoulda/cursor.hh"

namespace shoulda {

Cursor::Cursor(const CXCursor cursor)
    : cursor_(cursor) {
}

CXCursorKind Cursor::kind() const {
  return clang_getCursorKind(cursor_);
}

CXType Cursor::type() const {
  return clang_getCursorType(cursor_);
}

CXSourceLocation Cursor::location() const {
  return clang_getCursorLocation(cursor_);
}

}
