#ifndef INCLUDED_PKCS7_UNPAD_WRAP
#define INCLUDED_PKCS7_UNPAD_WRAP

#include <string>

namespace cryptopals {

std::string unpad_pkcs7(const std::string & input);

}  // namespace cryptopals

#endif
