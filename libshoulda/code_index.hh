#ifndef CODE_INDEX_HH
#define CODE_INDEX_HH

#include <clang-c/Index.h>

#include "libshoulda/compilation_database.hh"
#include "libshoulda/translation_unit.hh"

namespace shoulda {

class CodeIndex {
 public:
  CodeIndex();

  ~CodeIndex();

  TranslationUnit create_translation_unit(const CompileCommand& command);

 private:
  CXIndex index_;
};

}

#endif
