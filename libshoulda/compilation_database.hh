#ifndef COMPILATION_DATABASE_HH
#define COMPILATION_DATABASE_HH

#include <string>

#include <clang-c/CXCompilationDatabase.h>

namespace shoulda {

class CompilationDatabase {
 public:
  static CompilationDatabase from_directory(const std::string& path);

  ~CompilationDatabase();

  // TODO(nicholasbishop): remove this
  CXCompilationDatabase raw() const;

 private:
  CompilationDatabase(const CXCompilationDatabase database);

  const CXCompilationDatabase database_;
};

}

#endif
