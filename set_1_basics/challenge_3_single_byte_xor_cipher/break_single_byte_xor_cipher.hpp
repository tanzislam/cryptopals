#ifndef INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER
#define INCLUDED_BREAK_SINGLE_BYTE_XOR_CIPHER

#include <utility>
#include <cstdint>
#include <ostream>
#include <istream>

namespace cryptopals {

class break_single_byte_xor_cipher
{
  public:
    void rewind_and_decrypt_using_xor_byte(std::ostream & outputStream,
                                           std::istream & cipherTextStream,
                                           uint8_t xorByte);

    /// @return Pair of score and XOR byte
    virtual std::pair<unsigned int, uint8_t> do_break(
        std::istream & cipherTextStream) = 0;

    virtual ~break_single_byte_xor_cipher() {}
    break_single_byte_xor_cipher & get_ref() { return *this; }
};

}  // namespace cryptopals

#endif
