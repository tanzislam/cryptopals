#include "aes_ecb_encrypt.hpp"
#include <modes.h>
#include <aes.h>
#include <files.h>

namespace cryptopals {

void aes_ecb_encrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     bool addPaddingToInput)
{
    CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption aesEncryptor(
        reinterpret_cast<const unsigned char *>(key.data()), key.size());
    CryptoPP::FileSource(
        inputStream,
        true,
        new CryptoPP::StreamTransformationFilter(
            aesEncryptor,
            new CryptoPP::FileSink(outputStream),
            addPaddingToInput ? CryptoPP::BlockPaddingSchemeDef::DEFAULT_PADDING
                              : CryptoPP::BlockPaddingSchemeDef::NO_PADDING));
}

}  // namespace cryptopals
