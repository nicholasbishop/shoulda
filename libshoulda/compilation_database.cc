#include <string>

#include <clang-c/CXCompilationDatabase.h>

#include "libshoulda/compilation_database.hh"
#include "libshoulda/error.hh"

namespace shoulda {

CompilationDatabase CompilationDatabase::from_directory(
    const std::string& path) {
  CXCompilationDatabase_Error err;
  const auto database = clang_CompilationDatabase_fromDirectory(path.c_str(), &err);

  switch (err) {
    case CXCompilationDatabase_NoError:
      return CompilationDatabase(database);

    case CXCompilationDatabase_CanNotLoadDatabase:
      throw Error("couldn't load compilation database");
  }

  throw Error("unknown error creating compilation database");
}

CompilationDatabase::~CompilationDatabase() {
  clang_CompilationDatabase_dispose(database_);
}

CXCompilationDatabase CompilationDatabase::raw() const {
  return database_;
}

CompilationDatabase::CompilationDatabase(const CXCompilationDatabase database)
    : database_(database) {}

}
