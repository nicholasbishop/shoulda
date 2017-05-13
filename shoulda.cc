// adapted from http://shaharmike.com/cpp/libclang/

#include <iostream>
#include <vector>

#include <unistd.h>

#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>

#include "libshoulda/compilation_database.hh"

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;

using shoulda::CompilationDatabase;

ostream &operator<<(ostream &stream, const CXString &str) {
  stream << clang_getCString(str);
  clang_disposeString(str);
  return stream;
}

void outputError(CXCursor cursor, CXCursorKind parent_kind) {
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
       << column << ": warning: unused return value (parent is "
       << clang_getCursorKindSpelling(parent_kind) << ")"
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

  CXIndex index = clang_createIndex(1, 1);

  const auto all_cc = database.all_compile_commands();

  for (const auto cc : all_cc) {
    if (chdir(cc.working_directory().c_str()) != 0) {
      cerr << "chdir failed" << endl;
      exit(-1);
    }

    const auto num_command_line_args = clang_CompileCommand_getNumArgs(cc.raw());
    std::vector<CXString> free_later;

    std::vector<const char*> command_line_args;
    for (unsigned j = 0; j < num_command_line_args; ++j) {
      const auto arg = clang_CompileCommand_getArg(cc.raw(), j);
      command_line_args.emplace_back(clang_getCString(arg));
      free_later.emplace_back(arg);
    }

    for (const char *clarg : command_line_args) {
      cout << "  arg: " << clarg << endl;
    }

    CXTranslationUnit tu;
    const auto tu_err = clang_parseTranslationUnit2FullArgv(
        index, nullptr,
        command_line_args.data(), num_command_line_args,
        nullptr, 0,
        CXTranslationUnit_None,
        &tu);

    for (const auto str : free_later) {
      clang_disposeString(str);
    }

    switch (tu_err) {
      case CXError_Success:
        break;

      case CXError_Failure:
        cerr << "CXError_Failure" << endl;
        exit(-1);

      case CXError_Crashed:
        cerr << "CXError_Crashed" << endl;
        exit(-1);

      case CXError_InvalidArguments:
        cerr << "CXError_InvalidArguments" << endl;
        exit(-1);

      case CXError_ASTReadError:
        cerr << "CXError_ASTReadError" << endl;
        exit(-1);
    }
  
    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(
        cursor,
        [](CXCursor current, CXCursor parent, CXClientData) {
          // Function call
          if (clang_getCursorKind(current) == CXCursor_CallExpr) {

            // Function's return value is not void
            if (clang_getCursorType(current).kind != CXType_Void) {

              const auto parent_kind = clang_getCursorKind(parent);
              if (parent_kind != CXCursor_BinaryOperator &&
                  parent_kind != CXCursor_CallExpr &&
                  parent_kind != CXCursor_MemberRefExpr &&
                  parent_kind != CXCursor_ReturnStmt &&
                  parent_kind != CXCursor_TypedefDecl &&
                  parent_kind != CXCursor_VarDecl) {
                outputError(current, parent_kind);
              }
            }
          }
          return CXChildVisit_Recurse;
        },
        nullptr);
  }

  clang_disposeIndex(index);
}
