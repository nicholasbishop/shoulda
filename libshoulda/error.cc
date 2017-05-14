#include "libshoulda/error.hh"

namespace shoulda {

Error::Error(const char* what)
    : std::runtime_error(what) {
}

Unreachable::Unreachable()
    : Error("supposedly unreachable code reached") {
}

}
