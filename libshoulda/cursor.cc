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

Location Cursor::location() const {
  return Location::create(clang_getCursorLocation(cursor_));
}

std::vector<Location> Cursor::find_unused_return_values() const {
  std::vector<Location> result;

  visit_unused_return_values(
      [&result](Cursor current, Cursor) {
        result.emplace_back(current.location());
      });
  
  return result;
}

bool Cursor::is_unused_return_value(
    const Cursor& current, const Cursor& parent) {
  if (current.kind() != CXCursor_CallExpr) {
    // Not a function call
    return false;
  }

  // Function's return value is not void
  if (current.type().kind == CXType_Void) {
    // Function's return type is void
    return false;
  }

  const auto parent_kind = parent.kind();
  return (parent_kind != CXCursor_BinaryOperator &&
          parent_kind != CXCursor_CallExpr &&
          parent_kind != CXCursor_IfStmt &&
          parent_kind != CXCursor_MemberRefExpr &&
          parent_kind != CXCursor_ReturnStmt &&
          parent_kind != CXCursor_TypedefDecl &&
          parent_kind != CXCursor_VarDecl);
}

}
