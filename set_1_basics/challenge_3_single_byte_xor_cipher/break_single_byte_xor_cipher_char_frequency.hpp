#ifndef INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER_CHAR_FREQUENCY
#define INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER_CHAR_FREQUENCY

#include <utility>
#include <cstdint>
#include <istream>
#include <ostream>

namespace cryptopals {

/// @return Pair of score and XOR byte
std::pair<unsigned int, uint8_t> break_single_byte_xor_cipher_char_frequency(
        std::ostream & plainTextStream,
        std::istream & hexEncodedCipherTextStream
);

}  // close namespace cryptopals

#endif
