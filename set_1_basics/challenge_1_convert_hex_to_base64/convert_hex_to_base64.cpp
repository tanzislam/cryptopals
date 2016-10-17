#include "convert_hex_to_base64.hpp"
#include "decode_hex.hpp"
#include "encode_base64.hpp"

namespace cryptopals {

void convert_hex_to_base64(std::ostream & output, std::istream & input)
{
    decode_hex_streambuf hexDecoder(input);
    encode_base64_streambuf base64Encoder(output);
    std::istream(&hexDecoder).get(base64Encoder);
}

}  // close namespace cryptopals
