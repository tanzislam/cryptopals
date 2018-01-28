#ifndef INCLUDED_AES_ECB_ENCRYPT
#define INCLUDED_AES_ECB_ENCRYPT

#include <ostream>
#include <istream>
#include <string>

namespace cryptopals {

void aes_ecb_encrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     bool expectPadding = true);

}  // namespace cryptopals

#endif
