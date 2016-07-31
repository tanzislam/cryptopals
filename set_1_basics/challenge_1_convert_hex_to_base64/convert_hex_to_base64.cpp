#include "convert_hex_to_base64.hpp"
#include "decode_hex.hpp"
#include "encode_base64.hpp"
#include <istream>
#include <ostream>

namespace cryptopals {

void convert_hex_to_base64(std::ostream & output, std::istream & input)
{
    for (
            encode_base64::input_t buffer;
            input >> decode_hex(*(buffer[0] = 0u));
            buffer[0] = buffer[1] = buffer[2] = boost::none
    ) {
        decode_hex::decoded_t decodedValue;
        if (input >> decode_hex(decodedValue)) buffer[1] = decodedValue;
        if (input >> decode_hex(decodedValue)) buffer[2] = decodedValue;
        output << encode_base64(buffer);
    }
}

}  // close namespace cryptopals