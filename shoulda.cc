// adapted from http://shaharmike.com/cpp/libclang/

#include <iostream>
#include <vector>

#include <unistd.h>

#include <clang-c/Index.h>

#include "libshoulda/code_index.hh"
#include "libshoulda/compilation_database.hh"
#include "libshoulda/translation_unit.hh"
#include "libshoulda/util.hh"

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;

using shoulda::CodeIndex;
using shoulda::CompilationDatabase;
using shoulda::Cursor;
using shoulda::TranslationUnit;
using shoulda::is_directory;
using shoulda::to_string;

ostream &operator<<(ostream &stream, const CXString &str) {
  stream << to_string(str);
  return stream;
}

void outputError(Cursor cursor, Cursor parent) {
  const auto location = cursor.location();

  cout << location.path() << ":"
       << location.line() << ":"
       << location.column() << ": warning: unused return value (parent is "
       << clang_getCursorKindSpelling(parent.kind()) << ")"
       << endl;
}


void outputCursor(CXCursor c) {
  cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
       << clang_getCursorKindSpelling(clang_getCursorKind(c)) << "' of type "
       << clang_getTypeSpelling(clang_getCursorType(c)) << "'\n";
}

void show_errors(const TranslationUnit& translation_unit) {
  const auto cursor = translation_unit.cursor();
  cursor.visit_unused_return_values(
      [](const Cursor& current, const Cursor& parent) {
        // Skip system includes (TODO: find a more robust way to do
        // this)
        const auto path = current.location().path();
        if (path.length() >= 4 && path.substr(0, 4) == "/usr") {
          return;
        }

        outputError(current, parent);
      });
}

void check_compilation_database(const std::string& path) {
  const auto database = CompilationDatabase::from_directory(path);

  CodeIndex index;

  const auto all_cc = database.all_compile_commands();

  for (const auto cc : all_cc) {
    if (chdir(cc.working_directory().c_str()) != 0) {
      cerr << "chdir failed" << endl;
      exit(-1);
    }

    show_errors(index.translation_unit_from_command(cc));
  }
}

void check_single_file(const std::string& path) {
  CodeIndex index;
  show_errors(index.translation_unit_from_source_file(path));
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "usage: shoulda <compile-database-directory|source-file>" << endl;
    exit(-1);
  }
  const std::string path = argv[1];

  if (is_directory(path)) {
    check_compilation_database(path);
  } else {
    check_single_file(path);
  }
}
