#include "break_single_byte_xor_cipher.hpp"
#include <stdexcept>
#include "xor_combine.hpp"
#include "encode_hex.hpp"
#include "repeating_stringstream.hpp"

namespace cryptopals {

void break_single_byte_xor_cipher::rewindAndDecryptUsingXorByte(
        std::ostream & outputStream,
        std::istream & hexEncodedCipherTextStream,
        uint8_t xorByte
)
{
    hexEncodedCipherTextStream.clear();
    if (!hexEncodedCipherTextStream.seekg(0))
        throw std::runtime_error("Could not reset hexEncodedCipherTextStream");

    xor_combine(outputStream,
                hexEncodedCipherTextStream,
                reinterpret_cast<std::istream &>(
                        repeating_stringstream() << encode_hex(xorByte)
                ),
                false);
}

}  // close namespace cryptopals
