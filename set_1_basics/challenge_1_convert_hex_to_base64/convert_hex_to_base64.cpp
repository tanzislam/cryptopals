#include "convert_hex_to_base64.hpp"
#include "decode_hex.hpp"
#include "encode_base64.hpp"
#include <istream>
#include <ostream>

namespace cryptopals {

void convert_hex_to_base64(std::ostream & output, std::istream & input)
{
    decode_hex_streambuf hexDecoder(input);
    std::istream hexDecodedInput(&hexDecoder);
    hexDecodedInput.unsetf(std::ios_base::skipws);
    for (
            encode_base64::input_t buffer;
            hexDecodedInput >> *(buffer[0] = 0u);
            buffer[0] = buffer[1] = buffer[2] = boost::none
    ) {
        decode_hex::decoded_t decodedValue;
        if (hexDecodedInput >> decodedValue) buffer[1] = decodedValue;
        if (hexDecodedInput >> decodedValue) buffer[2] = decodedValue;
        output << encode_base64(buffer);
    }
}

}  // close namespace cryptopals
