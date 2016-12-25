#include "aes_cbc_decrypt.hpp"
#include <aes.h>
#include <sstream>
#include <strstream>
#include <cstring>
#include "tee.hpp"
#include "xor.hpp"
#include "aes_ecb_decrypt.hpp"

namespace cryptopals {

void aes_cbc_decrypt(std::ostream & outputStream,
                     std::istream & inputStream,
                     const std::string & key,
                     const char * initializationVector)
{
    char prevInput[CryptoPP::AES::BLOCKSIZE];
    for (
            std::istringstream prevInputStream(
                    std::string(initializationVector, CryptoPP::AES::BLOCKSIZE)
            );
            inputStream;
            prevInputStream.clear(),
                prevInputStream.str(std::string(prevInput, sizeof prevInput))
    ) {
        std::ostrstream thisInputSaver(prevInput, sizeof prevInput);
        tee_streambuf teeInput(inputStream, thisInputSaver);
        std::istream teeInputStream(&teeInput);
        xor_streambuf xorCombiner(outputStream, prevInputStream);
        aes_ecb_decrypt(std::ostream(&xorCombiner).flush(),
                        teeInputStream,
                        key,
                        false);
    }
}

}  // close namespace cryptopals
