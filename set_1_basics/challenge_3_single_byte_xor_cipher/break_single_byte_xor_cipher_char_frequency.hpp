#ifndef INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER_CHAR_FREQUENCY
#define INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER_CHAR_FREQUENCY

#include "break_single_byte_xor_cipher.hpp"
#include <map>

namespace cryptopals {

class break_single_byte_xor_cipher_char_frequency
        : public break_single_byte_xor_cipher
{
  public:
    std::pair<unsigned int, uint8_t> do_break(
        std::istream & cipherTextStream) override;
};

}  // namespace cryptopals

#endif
