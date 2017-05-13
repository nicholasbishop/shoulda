// adapted from http://shaharmike.com/cpp/libclang/

#include <iostream>
#include <vector>

#include <unistd.h>

#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>

#include "libshoulda/code_index.hh"
#include "libshoulda/compilation_database.hh"
#include "libshoulda/translation_unit.hh"

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;

using shoulda::CodeIndex;
using shoulda::CompilationDatabase;
using shoulda::Cursor;
using shoulda::TranslationUnit;

ostream &operator<<(ostream &stream, const CXString &str) {
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

void outputError(Cursor cursor, Cursor parent) {
  const auto location = cursor.location();

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
       << column << ": warning: unused return value (parent is "
       << clang_getCursorKindSpelling(parent.kind()) << ")"
       << endl;
}


void outputCursor(CXCursor c) {
  cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
       << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "' of type "
       << clang_getTypeSpelling(clang_getCursorType(c)) << "'\n";
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "usage: shoulda <compilation-database-directory>" << endl;
    exit(-1);
  }
  const char* path = argv[1];

  const auto database = CompilationDatabase::from_directory(path);

  CodeIndex index;

  const auto all_cc = database.all_compile_commands();

  for (const auto cc : all_cc) {
    if (chdir(cc.working_directory().c_str()) != 0) {
      cerr << "chdir failed" << endl;
      exit(-1);
    }

    const auto tu = index.create_translation_unit(cc);

    const auto cursor = tu.cursor();
    cursor.visit_children(
        [](Cursor current, Cursor parent) {
          // Function call
          if (current.kind() == CXCursor_CallExpr) {

            // Function's return value is not void
            if (current.type().kind != CXType_Void) {

              const auto parent_kind = parent.kind();
              if (parent_kind != CXCursor_BinaryOperator &&
                  parent_kind != CXCursor_CallExpr &&
                  parent_kind != CXCursor_MemberRefExpr &&
                  parent_kind != CXCursor_ReturnStmt &&
                  parent_kind != CXCursor_TypedefDecl &&
                  parent_kind != CXCursor_VarDecl) {
                outputError(current, parent);
              }
            }
          }
          return CXChildVisit_Recurse;
        });
  }
}
