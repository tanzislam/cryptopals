#include "aes_ecb_decrypt.hpp"
#include <modes.h>
#include <aes.h>
#include <files.h>

namespace cryptopals {

void aes_ecb_decrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     bool expectPadding)
{
    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption aesDecryptor(
        reinterpret_cast<const unsigned char *>(key.data()), key.size());
    CryptoPP::FileSource(
        inputStream,
        true,
        new CryptoPP::StreamTransformationFilter(
            aesDecryptor,
            new CryptoPP::FileSink(outputStream),
            expectPadding ? CryptoPP::BlockPaddingSchemeDef::DEFAULT_PADDING
                          : CryptoPP::BlockPaddingSchemeDef::NO_PADDING));
}

}  // namespace cryptopals
