#include "xor_combine.hpp"
#include "decode_hex.hpp"
#include "encode_hex.hpp"
#include "xor.hpp"

namespace cryptopals {

void xor_combine(std::ostream & hexEncodedOutputStream,
                 std::istream & hexEncodedInputStream1,
                 std::istream & hexEncodedInputStream2)
{
    decode_hex_streambuf hexDecoder1(hexEncodedInputStream1),
        hexDecoder2(hexEncodedInputStream2);
    std::istream inputStream1(&hexDecoder1), inputStream2(&hexDecoder2);
    xor_streambuf xorCombiner(inputStream1, inputStream2);
    encode_hex_streambuf hexEncoder(hexEncodedOutputStream);
    std::istream(&xorCombiner).get(hexEncoder);
}

}  // namespace cryptopals
