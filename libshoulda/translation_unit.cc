#include "libshoulda/translation_unit.hh"

namespace shoulda {

TranslationUnit::~TranslationUnit() {
  clang_disposeTranslationUnit(translation_unit_);
}

Cursor TranslationUnit::cursor() const {
  return Cursor(clang_getTranslationUnitCursor(translation_unit_));
}

TranslationUnit::TranslationUnit(const CXTranslationUnit translation_unit)
    : translation_unit_(translation_unit) {
}

}
