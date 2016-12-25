#include "aes_ecb_encrypt.hpp"
#include <modes.h>
#include <aes.h>
#include <files.h>

namespace cryptopals {

void aes_ecb_encrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key)
{
    CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption aesEncryptor(
            reinterpret_cast<const unsigned char *>(key.data()),
            key.size()
    );
    CryptoPP::FileSource(
            inputStream,
            true,
            new CryptoPP::StreamTransformationFilter(
                    aesEncryptor,
                    new CryptoPP::FileSink(outputStream)
            )
    );
}

}  // close namespace cryptopals
