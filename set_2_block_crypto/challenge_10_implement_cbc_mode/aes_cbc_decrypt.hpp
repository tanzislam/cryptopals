#ifndef INCLUDED_AES_CBC_DECRYPT
#define INCLUDED_AES_CBC_DECRYPT

#include <ostream>
#include <istream>
#include <string>

namespace cryptopals {

void aes_cbc_decrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     const char * initializationVector);

}  // namespace cryptopals

#endif
