#ifndef TRANSLATION_UNIT_HH
#define TRANSLATION_UNIT_HH

#include <clang-c/Index.h>

#include "libshoulda/command_line.hh"
#include "libshoulda/cursor.hh"
#include "libshoulda/location.hh"

namespace shoulda {

class UnusedReturnValue {
 public:

  // TODO not sure what info is really needed yet
  const Location location;
  const CXCursorKind parent_kind;

  UnusedReturnValue(const Location location, const CXCursorKind parent_kind)
      : location(location), parent_kind(parent_kind) {
  }
};

class TranslationUnit {
 public:
  TranslationUnit(const CXTranslationUnit translation_unit);

  ~TranslationUnit();

  // TODO add visitor interface?
  std::vector<UnusedReturnValue> find_unused_return_values() const;

  Cursor cursor() const;

 private:
  

  const CXTranslationUnit translation_unit_;
};

}

#endif
