#ifndef INCLUDED_AES_ECB_DECRYPT
#define INCLUDED_AES_ECB_DECRYPT

#include <ostream>
#include <istream>
#include <string>

namespace cryptopals {

void aes_ecb_decrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     bool expectPadding = true);

}  // namespace cryptopals

#endif
