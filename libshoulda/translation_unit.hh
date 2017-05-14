#ifndef TRANSLATION_UNIT_HH
#define TRANSLATION_UNIT_HH

#include <clang-c/Index.h>

#include "libshoulda/command_line.hh"
#include "libshoulda/cursor.hh"

namespace shoulda {

class TranslationUnit {
 public:
  TranslationUnit(const CXTranslationUnit translation_unit);

  ~TranslationUnit();

  Cursor cursor() const;

 private:
  const CXTranslationUnit translation_unit_;
};

}

#endif
