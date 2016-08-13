#include "decode_hex.hpp"
#include <string>
#include <istream>
#include <iomanip>
#include <cstdlib>

namespace cryptopals {

std::istream & operator>>(std::istream & input, const decode_hex & manipulator)
{
    std::string hexInput;
    hexInput.reserve(decode_hex::s_numHexDigitsInOctet);

    input >> std::setw(decode_hex::s_numHexDigitsInOctet) >> hexInput;
    if (!input) return input;

    if (hexInput.size() != decode_hex::s_numHexDigitsInOctet) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    manipulator.d_value = std::stoul(hexInput.c_str(),
                                     0,
                                     1 << decode_hex::s_numBitsInHexDigit);
    return input;
}

}  // close namespace cryptopals
