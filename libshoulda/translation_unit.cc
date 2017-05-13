#include "libshoulda/error.hh"
#include "libshoulda/translation_unit.hh"

namespace shoulda {

namespace {

std::vector<const char*> raw_command_line(const CommandLine& input) {
  std::vector<const char*> output;
  output.reserve(input.size());
  for (const auto& arg : input) {
    output.emplace_back(arg.c_str());
  }
  return output;
}

}  // namespace

TranslationUnit TranslationUnit::from_command_line(
    const CXIndex index, const CommandLine& command_line) {
  std::vector<const char*> args = raw_command_line(command_line);

  const char* const source_filename = nullptr;
  CXUnsavedFile* const unsaved_files = nullptr;
  const unsigned num_unsaved_files = 0;
  const unsigned options = CXTranslationUnit_None;

  CXTranslationUnit translation_unit;
  const auto err = clang_parseTranslationUnit2FullArgv(
      index,
      source_filename,
      args.data(),
      args.size(),
      unsaved_files,
      num_unsaved_files,
      options,
      &translation_unit);

  switch (err) {
    case CXError_Success:
      return TranslationUnit(translation_unit);

    case CXError_Failure:
      throw Error("CXError_Failure");

    case CXError_Crashed:
      throw Error("CXError_Crashed");

    case CXError_InvalidArguments:
      throw Error("CXError_InvalidArguments");

    case CXError_ASTReadError:
      throw Error("CXError_ASTReadError");
  }

  throw Error("unexpected error from clang_parseTranslationUnit2FullArgv");
}

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
