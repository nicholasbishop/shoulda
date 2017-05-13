#ifndef COMPILATION_DATABASE_HH
#define COMPILATION_DATABASE_HH

#include <string>

#include <clang-c/CXCompilationDatabase.h>

namespace shoulda {

class CompileCommands {
 public:
  class Iter {
   public:
    explicit Iter(const CompileCommands& parent, const size_t index);

    bool operator!=(const Iter& other) const;

    Iter& operator++();

    // TODO, wrap type
    CXCompileCommand operator*() const;

   private:
    const CompileCommands& parent_;
    size_t index_;
  };

  CompileCommands(const CXCompileCommands commands);

  ~CompileCommands();

  // TODO, wrap type
  CXCompileCommand operator[](const size_t index) const;

  Iter begin() const;

  Iter end() const;

 private:
  const CXCompileCommands commands_;
  const size_t size_;
};

class CompilationDatabase {
 public:
  static CompilationDatabase from_directory(const std::string& path);

  CompileCommands all_compile_commands() const;

  ~CompilationDatabase();

 private:
  CompilationDatabase(const CXCompilationDatabase database);

  const CXCompilationDatabase database_;
};

}

#endif
