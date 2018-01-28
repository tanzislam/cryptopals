#ifndef INCLUDED_DETECT_SINGLE_BYTE_XOR_CIPHER
#define INCLUDED_DETECT_SINGLE_BYTE_XOR_CIPHER

#include <tuple>
#include <cstdint>
#include <ostream>
#include <istream>
#include "break_single_byte_xor_cipher.hpp"

namespace cryptopals {

/** @return Line number (zero-based), Score, XOR byte*/
std::tuple<unsigned int, unsigned int, uint8_t> detect_single_byte_xor_cipher(
    std::ostream & output,
    std::istream & input,
    break_single_byte_xor_cipher & mechanism);

}  // namespace cryptopals

#endif
