#include "break_single_byte_xor_cipher.hpp"
#include <stdexcept>

namespace cryptopals {

void break_single_byte_xor_cipher::rewindAndDecryptUsingXorByte(
        std::ostream & outputStream,
        std::istream & cipherTextStream,
        uint8_t xorByte
)
{
    cipherTextStream.clear();
    if (!cipherTextStream.seekg(0))
        throw std::runtime_error("Could not reset hexEncodedCipherTextStream");

    char value1;
    while (cipherTextStream.get(value1))
        outputStream << char(value1 ^ xorByte);
}

}  // close namespace cryptopals
