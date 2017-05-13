#ifndef TRANSLATION_UNIT_HH
#define TRANSLATION_UNIT_HH

#include <clang-c/Index.h>

#include "libshoulda/command_line.hh"

namespace shoulda {

class TranslationUnit {
 public:
  // TODO wrap CXIndex?
  static TranslationUnit from_command_line(
      const CXIndex index, const CommandLine& command_line);

  ~TranslationUnit();

  // TODO
  CXTranslationUnit raw() const;

 private:
  TranslationUnit(const CXTranslationUnit translation_unit);

  const CXTranslationUnit translation_unit_;
};

}

#endif
