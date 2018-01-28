#ifndef INCLUDED_AES_CBC_ENCRYPT
#define INCLUDED_AES_CBC_ENCRYPT

#include <ostream>
#include <istream>
#include <string>

namespace cryptopals {

void aes_cbc_encrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     const char * initializationVector);

}  // namespace cryptopals

#endif
