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

std::vector<Cursor> Cursor::get_children() const {
  using Children = std::vector<Cursor>;
  Children children;
  CXClientData client_data = static_cast<CXClientData>(&children);
  clang_visitChildren(
        cursor_,
        [](CXCursor current, CXCursor, CXClientData client_data) {
          Children* children = static_cast<Children*>(client_data);
          children->emplace_back(current);
          return CXChildVisit_Continue;
        },
        client_data);
  return children;
}

bool Cursor::operator==(const Cursor& other) const {
  const auto loc = Location::create(clang_getCursorLocation(cursor_));
  const auto other_loc = Location::create(
      clang_getCursorLocation(other.cursor_));
  return loc == other_loc;
}

bool Cursor::is_unused_return_value(
    const Cursor& current, const Cursor& parent) {
  if (current.kind() != CXCursor_CallExpr) {
    // Not a function call
    return false;
  }

  // Ignore functions that don't return anything
  if (current.type().kind == CXType_Void) {
    return false;
  }

  const auto parent_kind = parent.kind();

  const auto siblings = parent.get_children();
  if (parent_kind == CXCursor_IfStmt) {
    // If the return value is the conditional it's used. If it's the
    // unbraced body of the |if| then the value isn't used.
    return current == siblings[1];
  }

  return (parent_kind != CXCursor_BinaryOperator &&
          parent_kind != CXCursor_CallExpr &&
          parent_kind != CXCursor_MemberRefExpr &&
          parent_kind != CXCursor_ReturnStmt &&
          parent_kind != CXCursor_TypedefDecl &&
          parent_kind != CXCursor_VarDecl);
}

}
