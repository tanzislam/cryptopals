#include "ecb_encryption_oracle.hpp"
#include "generate_random.hpp"
#include <aes.h>
#include <strstream>
#include "decode_base64.hpp"
#include "cat.hpp"
#include "aes_ecb_encrypt.hpp"

namespace cryptopals {

extern const char * const base64EncodedSuffix =
    "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
    "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
    "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
    "YnkK";


void ecbEncryptOracle(std::ostream & outputStream, std::istream & inputStream)
{
    static auto key = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);

    std::istrstream base64EncodedSuffixStream(base64EncodedSuffix);

    decode_base64_streambuf suffix(base64EncodedSuffixStream);
    std::istream suffixStream(&suffix);

    cat_streambuf inputAndSuffix(inputStream, suffixStream);
    std::istream inputAndSuffixStream(&inputAndSuffix);

    aes_ecb_encrypt(outputStream, inputAndSuffixStream, key);
}

}  // namespace cryptopals
