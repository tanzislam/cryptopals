#include "detect_encryption_method.hpp"
#include <sstream>
#include <aes.h>

namespace cryptopals {

EncryptionMode detectEncryptionMode(void (*blackBox)(std::ostream &,
                                                     std::istream &))
{
    std::ostringstream cipherTextStream;
    std::istringstream plainTextStream(
        std::string(5 * CryptoPP::AES::BLOCKSIZE, '0'));
    blackBox(cipherTextStream, plainTextStream);

    cipherTextStream.flush();
    const auto & cipherText = cipherTextStream.str();
    if (cipherText.compare(CryptoPP::AES::BLOCKSIZE,
                           CryptoPP::AES::BLOCKSIZE,
                           cipherText,
                           2 * CryptoPP::AES::BLOCKSIZE,
                           CryptoPP::AES::BLOCKSIZE))
        return EncryptionMode::CBC;
    else
        return EncryptionMode::ECB;
}

}  // namespace cryptopals
