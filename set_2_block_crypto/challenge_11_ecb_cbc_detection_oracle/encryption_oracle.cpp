#include "encryption_oracle.hpp"
#include "generate_random.hpp"
#include <sstream>
#include "cat.hpp"
#include <aes.h>
#include "pkcs7_pad.hpp"
#include "aes_ecb_encrypt.hpp"
#include "aes_cbc_encrypt.hpp"

namespace cryptopals {

enum struct EncryptionMode { ECB, CBC } lastEncryptionMode;


void encryptionOracle(std::ostream & outputStream, std::istream & inputStream)
{
    std::uniform_int_distribution<unsigned short> prefixAndSuffixLength(5, 10);

    std::istringstream prefix(
        generateRandomBytes(prefixAndSuffixLength(getRandomNumbers())));

    cat_streambuf prefixThenInput(prefix, inputStream);
    std::istream prefixAndInput(&prefixThenInput);

    std::istringstream suffix(
        generateRandomBytes(prefixAndSuffixLength(getRandomNumbers())));

    cat_streambuf prefixThenInputThenSuffix(prefixAndInput, suffix);
    std::istream prefixAndInputAndSuffix(&prefixThenInputThenSuffix);

    std::uniform_int_distribution<unsigned short> decideEncryptionMethod(0, 1);
    if (decideEncryptionMethod(getRandomNumbers())) {
        lastEncryptionMode = EncryptionMode::ECB;
        pkcs7_pad_streambuf pkcs7Padder(prefixAndInputAndSuffix,
                                        CryptoPP::AES::BLOCKSIZE);
        std::istream pkcs7PaddedInput(&pkcs7Padder);
        aes_ecb_encrypt(outputStream,
                        pkcs7PaddedInput,
                        generateRandomBytes(CryptoPP::AES::BLOCKSIZE));
    } else {
        lastEncryptionMode = EncryptionMode::CBC;
        aes_cbc_encrypt(outputStream,
                        prefixAndInputAndSuffix,
                        generateRandomBytes(CryptoPP::AES::BLOCKSIZE),
                        generateRandomBytes(CryptoPP::AES::BLOCKSIZE).c_str());
    }
}

}  // namespace cryptopals
