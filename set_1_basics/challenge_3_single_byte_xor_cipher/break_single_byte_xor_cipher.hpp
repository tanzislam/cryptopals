#ifndef INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER
#define INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER

#include <cstdint>
#include <iosfwd>

namespace cryptopals {

uint8_t break_single_byte_xor_cipher(std::ostream & plainTextStream,
                                     std::istream & hexEncodedCipherTextStream);

}  // close namespace cryptopals

#endif
