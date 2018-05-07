#include "ecb_encryption_oracle_with_prefix.hpp"
#include "generate_random.hpp"
#include <aes.h>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "decode_base64.hpp"
#include <cstring>
#include "cat.hpp"
#include "aes_ecb_encrypt.hpp"

namespace cryptopals {

extern const char * const base64EncodedSuffix2 =
    "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
    "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
    "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
    "YnkK";


void ecbEncryptOracle2(std::ostream & outputStream, std::istream & inputStream)
{
    static auto prefix = generateRandomBytes(
        std::uniform_int_distribution<unsigned short>(5,
                                                      15)(getRandomNumbers()));
    boost::iostreams::stream<boost::iostreams::array_source>
        prefixStream(prefix.c_str(), prefix.size());

    cat_streambuf prefixThenInput(prefixStream, inputStream);
    std::istream prefixAndInput(&prefixThenInput);

    boost::iostreams::stream<boost::iostreams::array_source>
        base64EncodedSuffix2Stream(base64EncodedSuffix2,
                                   std::strlen(base64EncodedSuffix2));

    decode_base64_streambuf suffix(base64EncodedSuffix2Stream);
    std::istream suffixStream(&suffix);

    cat_streambuf prefixAndInputThenSuffix(prefixAndInput, suffixStream);
    std::istream prefixAndInputAndSuffix(&prefixAndInputThenSuffix);

    static auto key = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
    aes_ecb_encrypt(outputStream, prefixAndInputAndSuffix, key);
}

}  // namespace cryptopals
