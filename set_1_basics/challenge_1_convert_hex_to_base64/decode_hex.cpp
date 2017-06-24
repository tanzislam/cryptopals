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
    : d_inputStream(inputStream), d_buffer(), d_startPos(inputStream.tellg())
{
    if (!d_inputStream || d_startPos == std::streampos(-1))
        throw std::ios_base::failure("Could not obtain initial position");
    setg(reinterpret_cast<char*>(&d_buffer), nullptr, nullptr);
}


std::streambuf::pos_type decode_hex_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    assert(pos % sizeof(d_buffer) == 0);
    assert(which == std::ios_base::in);
    return
            pos % sizeof(d_buffer) == 0
            && which == std::ios_base::in
            && (d_inputStream.clear(),
                d_inputStream.seekg(d_startPos
                                    + pos * decode_hex::s_numHexDigitsInOctet))
        ? pos
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type decode_hex_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    assert(abs(off) % sizeof(d_buffer) == 0);
    assert(dir == std::ios_base::cur);
    assert(which == std::ios_base::in);
    assert(-off <= (d_inputStream.tellg() - d_startPos)
                            / decode_hex::s_numHexDigitsInOctet);
    return
            abs(off) % sizeof(d_buffer) == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
            && -off <= (d_inputStream.tellg() - d_startPos)
                                / decode_hex::s_numHexDigitsInOctet
            && (d_inputStream.clear(),
                d_inputStream.seekg(off * decode_hex::s_numHexDigitsInOctet,
                                    dir))
        ? std::streampos((d_inputStream.tellg() - d_startPos)
                                / decode_hex::s_numHexDigitsInOctet)
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type decode_hex_streambuf::underflow()
{
    try {
        if (d_inputStream >> decode_hex(d_buffer)) {
            setg(reinterpret_cast<char *>(&d_buffer),
                 reinterpret_cast<char *>(&d_buffer),
                 reinterpret_cast<char *>(&d_buffer) + sizeof(d_buffer));
            return std::streambuf::traits_type::to_int_type(d_buffer);
        } else return std::streambuf::traits_type::eof();
    } catch (...) {
        return std::streambuf::traits_type::eof();
    }
}


}  // close namespace cryptopals
