#include <string>

#include <clang-c/CXCompilationDatabase.h>

#include "libshoulda/compilation_database.hh"
#include "libshoulda/error.hh"

namespace shoulda {

namespace {
std::string to_string(const CXString input) {
  const std::string output = clang_getCString(input);
  clang_disposeString(input);
  return output;
}
}

CompileCommand::CompileCommand(const CXCompileCommand command)
    : command_(command) {
}

std::string CompileCommand::working_directory() const {
  return to_string(clang_CompileCommand_getDirectory(command_));
}

// TODO
CXCompileCommand CompileCommand::raw() const {
  return command_;
}

CompileCommands::Iter::Iter(const CompileCommands& parent, const size_t index)
    : parent_(parent), index_(index) {
}

bool CompileCommands::Iter::operator!=(const Iter& other) const {
  return (&parent_ != &other.parent_) || (index_ != other.index_);
}

CompileCommands::Iter& CompileCommands::Iter::operator++() {
  ++index_;
  return *this;
}

CompileCommand CompileCommands::Iter::operator*() const {
  return parent_[index_];
}

CompileCommands::CompileCommands(const CXCompileCommands commands)
    : commands_(commands),
      size_(clang_CompileCommands_getSize(commands_)) {
}

CompileCommands::~CompileCommands() {
  clang_CompileCommands_dispose(commands_);
}

CompileCommand CompileCommands::operator[](const size_t index) const {
  if (index < size_) {
    return CompileCommand(clang_CompileCommands_getCommand(commands_, index));
  } else {
    throw Error("index out of range");
  }
}

CompileCommands::Iter CompileCommands::begin() const {
  return Iter(*this, 0);
}

CompileCommands::Iter CompileCommands::end() const {
  return Iter(*this, size_);
}

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

CompileCommands CompilationDatabase::all_compile_commands() const {
  return CompileCommands(
      clang_CompilationDatabase_getAllCompileCommands(database_));
}

CompilationDatabase::~CompilationDatabase() {
  clang_CompilationDatabase_dispose(database_);
}

CompilationDatabase::CompilationDatabase(const CXCompilationDatabase database)
    : database_(database) {}

}
