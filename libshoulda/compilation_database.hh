#ifndef COMPILATION_DATABASE_HH
#define COMPILATION_DATABASE_HH

#include <string>

#include <clang-c/CXCompilationDatabase.h>

#include "libshoulda/command_line.hh"

namespace shoulda {

class CompileCommand {
 public:
  CompileCommand(const CXCompileCommand command);

  std::string working_directory() const;

  CommandLine command_line() const;

 private:
  const CXCompileCommand command_;
};

class CompileCommands {
 public:
  class Iter {
   public:
    explicit Iter(const CompileCommands& parent, const size_t index);

    bool operator!=(const Iter& other) const;

    Iter& operator++();

    CompileCommand operator*() const;

   private:
    const CompileCommands& parent_;
    size_t index_;
  };

  CompileCommands(const CXCompileCommands commands);

  ~CompileCommands();

  CompileCommand operator[](const size_t index) const;

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
