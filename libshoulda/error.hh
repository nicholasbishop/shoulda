#ifndef ERROR_HH
#define ERROR_HH

#include <stdexcept>

namespace shoulda {

class Error : public std::runtime_error {
 public:
  Error(const char* what);
};

}

#endif
