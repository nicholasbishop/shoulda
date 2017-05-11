// adapted from http://shaharmike.com/cpp/libclang/

#include <iostream>
#include <vector>

#include <clang-c/CXCompilationDatabase.h>
#include <clang-c/Index.h>

#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;

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

  CXCompilationDatabase_Error cdb_err;
  const auto cdb = clang_CompilationDatabase_fromDirectory(path, &cdb_err);

  switch (cdb_err) {
    case CXCompilationDatabase_NoError:
      break;

    case CXCompilationDatabase_CanNotLoadDatabase:
      cerr << "unable to load compilation database" << endl;
      exit(-1);
  }

  CXIndex index = clang_createIndex(1, 1);

  const auto all_cc = clang_CompilationDatabase_getAllCompileCommands(cdb);
  const auto num_cc = clang_CompileCommands_getSize(all_cc);

  for (unsigned i = 0; i < num_cc; ++i) {
    const auto cc = clang_CompileCommands_getCommand(all_cc, i);

    const auto source_filename = clang_CompileCommand_getFilename(cc);
    const auto wd = clang_CompileCommand_getDirectory(cc);

    if (chdir(clang_getCString(wd)) != 0) {
      cerr << "chdir failed" << endl;
      exit(-1);
    }

    cout << "source: " << clang_getCString(source_filename) << endl;

    const auto num_command_line_args = clang_CompileCommand_getNumArgs(cc);
    std::vector<CXString> free_later;
    free_later.emplace_back(source_filename);
    free_later.emplace_back(wd);

    std::vector<const char*> command_line_args;
    for (unsigned j = 0; j < num_command_line_args; ++j) {
      const auto arg = clang_CompileCommand_getArg(cc, j);
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

  clang_CompileCommands_dispose(all_cc);
  clang_CompilationDatabase_dispose(cdb);
  clang_disposeIndex(index);
}
