#ifndef CODE_INDEX_HH
#define CODE_INDEX_HH

#include <clang-c/Index.h>

#include "libshoulda/command_line.hh"
#include "libshoulda/compilation_database.hh"
#include "libshoulda/translation_unit.hh"

namespace shoulda {

class CodeIndex {
 public:
  CodeIndex();

  ~CodeIndex();

  TranslationUnit translation_unit_from_command(
      const CompileCommand& command);

  TranslationUnit translation_unit_from_command_line(
      const CommandLine& command_line);

  TranslationUnit translation_unit_from_source_file(
      const std::string& path);

 private:
  CXIndex index_;
};

}

#endif
