#include "libshoulda/translation_unit.hh"

namespace shoulda {

TranslationUnit::TranslationUnit(const CXTranslationUnit translation_unit)
    : translation_unit_(translation_unit) {
}

TranslationUnit::~TranslationUnit() {
  clang_disposeTranslationUnit(translation_unit_);
}

Cursor TranslationUnit::cursor() const {
  return Cursor(clang_getTranslationUnitCursor(translation_unit_));
}

std::vector<UnusedReturnValue>
TranslationUnit::find_unused_return_values() const {
  std::vector<UnusedReturnValue> result;

  cursor().visit_children(
      [&result](Cursor current, Cursor parent) {
        // Function call
        if (current.kind() == CXCursor_CallExpr) {

          // Function's return value is not void
          if (current.type().kind != CXType_Void) {

            const auto parent_kind = parent.kind();
            if (parent_kind != CXCursor_BinaryOperator &&
                parent_kind != CXCursor_CallExpr &&
                parent_kind != CXCursor_IfStmt &&
                parent_kind != CXCursor_MemberRefExpr &&
                parent_kind != CXCursor_ReturnStmt &&
                parent_kind != CXCursor_TypedefDecl &&
                parent_kind != CXCursor_VarDecl) {
              result.emplace_back(UnusedReturnValue(current.location(),
                                                    parent_kind));
            }
          }
        }
        return CXChildVisit_Recurse;
      });

  return result;
}


}
