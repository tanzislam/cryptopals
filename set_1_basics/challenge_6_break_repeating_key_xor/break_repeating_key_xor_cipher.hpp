#ifndef INCLUDED_BREAK_REPEATING_KEY_XOR_CIPHER
#define INCLUDED_BREAK_REPEATING_KEY_XOR_CIPHER

#include <utility>
#include <string>
#include <istream>
#include <ostream>

namespace cryptopals {

std::pair<unsigned int, std::string> break_repeating_key_xor_cipher(
        std::istream & inputStream
);

void rewind_and_decrypt_using_repeating_xor_key(std::ostream & outputStream,
                                                std::istream & cipherTextStream,
                                                const std::string & xorKey);

}  // close namespace cryptopals

#endif
