#include "xor_combine.hpp"
#include "decode_hex.hpp"
#include "encode_hex.hpp"
#include <istream>
#include <ostream>
#include <iomanip>

namespace cryptopals {

void xor_combine(std::ostream & outputStream,
                 std::istream & hexEncodedInputStream1,
                 std::istream & hexEncodedInputStream2,
                 bool hexEncodeOutput)
{
    decode_hex::decoded_t value1, value2;
    while (
            hexEncodedInputStream1 >> decode_hex(value1)
            && hexEncodedInputStream2 >> decode_hex(value2)
    ) {
        if (hexEncodeOutput) outputStream << encode_hex(value1 ^ value2);
        else outputStream << char(value1 ^ value2);
    }
}

}  // close namespace cryptopals
