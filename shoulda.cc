// adapted from http://shaharmike.com/cpp/libclang/

#include <clang-c/Index.h> // This is libclang.
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;

ostream &operator<<(ostream &stream, const CXString &str) {
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

void outputError(CXCursor cursor) {
  const auto location = clang_getCursorLocation(cursor);

  CXFile file;
  unsigned line;
  unsigned column;
  unsigned offset;

  clang_getSpellingLocation(location,
                            &file,
                            &line,
                            &column,
                            &offset);
  cout << clang_getFileName(file) << ":"
       << line << ":"
       << column << ": warning: unused return value\n";
}


void outputCursor(CXCursor c) {
  cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
       << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "' of type "
       << clang_getTypeSpelling(clang_getCursorType(c)) << "'\n";
}

int main() {
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit unit = clang_parseTranslationUnit(
      index, "calltest.cc", nullptr, 0, nullptr, 0, CXTranslationUnit_None);
  if (unit == nullptr) {
    cerr << "Unable to parse translation unit. Quitting." << endl;
    exit(-1);
  }

  CXCursor cursor = clang_getTranslationUnitCursor(unit);
  clang_visitChildren(
      cursor,
      [](CXCursor current, CXCursor parent, CXClientData) {
        // Function call
        if (clang_getCursorKind(current) == CXCursor_CallExpr) {

          // Function's return value is not void
          if (clang_getCursorType(current).kind != CXType_Void) {

            const auto parent_kind = clang_getCursorKind(parent);
            if (parent_kind != CXCursor_CallExpr &&
                parent_kind != CXCursor_ReturnStmt &&
                parent_kind != CXCursor_VarDecl) {
              outputError(current);
            }
          }
        }
        return CXChildVisit_Recurse;
      },
      nullptr);

  clang_disposeTranslationUnit(unit);
  clang_disposeIndex(index);
}
