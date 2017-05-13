#include "libshoulda/code_index.hh"

namespace shoulda {

CodeIndex::CodeIndex()
    // TODO, params
    : index_(clang_createIndex(1, 1)) {
}

CodeIndex::~CodeIndex() {
  clang_disposeIndex(index_);
}

TranslationUnit CodeIndex::create_translation_unit(const CompileCommand& command) {
  return TranslationUnit::from_command_line(index_, command.command_line());
}

}
