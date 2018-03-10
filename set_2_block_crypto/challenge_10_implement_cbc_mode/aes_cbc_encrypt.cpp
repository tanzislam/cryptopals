#include "aes_cbc_encrypt.hpp"
#include "pkcs7_pad.hpp"
#include <aes.h>
#include <sstream>
#include "xor.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "tee.hpp"
#include "aes_ecb_encrypt.hpp"

namespace cryptopals {

void aes_cbc_encrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     const char * initializationVector)
{
    pkcs7_pad_streambuf pkcs7Padder(inputStream, CryptoPP::AES::BLOCKSIZE);
    std::istream pkcs7PaddedInput(&pkcs7Padder);
    char prevResult[CryptoPP::AES::BLOCKSIZE];
    for (std::istringstream prevResultStream(
             std::string(initializationVector, CryptoPP::AES::BLOCKSIZE));
         pkcs7PaddedInput;
         prevResultStream.clear(),
         prevResultStream.str(std::string(prevResult, sizeof prevResult))) {
        xor_streambuf xorCombiner(pkcs7PaddedInput, prevResultStream);
        std::istream xorCombinedInput(&xorCombiner);
        boost::iostreams::stream<boost::iostreams::array_sink>
            thisOutputSaver(prevResult, sizeof prevResult);
        tee_streambuf teeOutput(thisOutputSaver, outputStream);
        aes_ecb_encrypt(std::ostream(&teeOutput).flush(),
                        xorCombinedInput,
                        key,
                        false);
    }
}

}  // namespace cryptopals
