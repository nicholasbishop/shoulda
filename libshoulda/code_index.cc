#include <clang-c/Index.h>

#include "libshoulda/code_index.hh"
#include "libshoulda/error.hh"

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

void throw_on_cx_error(const CXErrorCode err) {
  switch (err) {
    case CXError_Success:
      return;

    case CXError_Failure:
      throw Error("CXError_Failure");

    case CXError_Crashed:
      throw Error("CXError_Crashed");

    case CXError_InvalidArguments:
      throw Error("CXError_InvalidArguments");

    case CXError_ASTReadError:
      throw Error("CXError_ASTReadError");
  }

  throw Unreachable();
}

}  // namespace

CodeIndex::CodeIndex()
    // TODO, params
    : index_(clang_createIndex(1, 1)) {
}

CodeIndex::~CodeIndex() {
  clang_disposeIndex(index_);
}

TranslationUnit CodeIndex::translation_unit_from_command(
    const CompileCommand& command) {
  return translation_unit_from_command_line(command.command_line());
}

TranslationUnit CodeIndex::translation_unit_from_command_line(
    const CommandLine& command_line) {
  std::vector<const char*> args = raw_command_line(command_line);

  const char* const source_filename = nullptr;
  CXUnsavedFile* const unsaved_files = nullptr;
  const unsigned num_unsaved_files = 0;
  const unsigned options = CXTranslationUnit_None;

  CXTranslationUnit translation_unit;
  const auto err = clang_parseTranslationUnit2FullArgv(
      index_,
      source_filename,
      args.data(),
      args.size(),
      unsaved_files,
      num_unsaved_files,
      options,
      &translation_unit);

  throw_on_cx_error(err);

  return TranslationUnit(translation_unit);
}

TranslationUnit CodeIndex::translation_unit_from_source_file(
    const std::string& path) {
  const char* const* command_line_args = nullptr;
  const int num_command_line_args = 0;
  CXUnsavedFile* const unsaved_files = nullptr;
  const unsigned num_unsaved_files = 0;
  const unsigned options = CXTranslationUnit_None;

  CXTranslationUnit translation_unit;

  const auto err = clang_parseTranslationUnit2(
      index_,
      path.c_str(),
      command_line_args,
      num_command_line_args,
      unsaved_files,
      num_unsaved_files,
      options,
      &translation_unit);

  throw_on_cx_error(err);

  return TranslationUnit(translation_unit);
}

}
