#include "decode_hex.hpp"
#include <string>
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


decode_hex_streambuf::decode_hex_streambuf(std::istream & inputStream)
    : d_inputStream(inputStream), d_startPos(inputStream.tellg())
{
    setg(reinterpret_cast<char*>(&d_buffer), nullptr, nullptr);
}


std::streambuf::pos_type decode_hex_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    return
            pos % sizeof(d_buffer) == 0
            && which == std::ios_base::in
            && (d_inputStream.clear(),
                d_inputStream.seekg(d_startPos
                                    + pos * decode_hex::s_numHexDigitsInOctet))
        ? pos
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::int_type decode_hex_streambuf::underflow()
{
    if (d_inputStream >> decode_hex(d_buffer)) {
        setg(reinterpret_cast<char *>(&d_buffer),
             reinterpret_cast<char *>(&d_buffer),
             reinterpret_cast<char *>(&d_buffer) + sizeof(d_buffer));
        return std::streambuf::traits_type::to_int_type(d_buffer);
    } else return std::streambuf::traits_type::eof();
}


}  // close namespace cryptopals
