#ifndef INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER_RECOGNIZE_WORDS
#define INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER_RECOGNIZE_WORDS

#include "break_single_byte_xor_cipher.hpp"

namespace cryptopals {

class break_single_byte_xor_cipher_recognize_words
    : public break_single_byte_xor_cipher
{
  public:
    std::pair<unsigned int, uint8_t> do_break(std::ostream & outputStream,
                                              std::istream & cipherTextStream);
};

}  // close namespace cryptopals

#endif
