#include "encode_hex_streambuf.hpp"
#include <strstream>
#include "encode_hex.hpp"

namespace cryptopals {

encode_hex_streambuf::encode_hex_streambuf(std::istream & inputStream)
    : d_inputStream(inputStream), d_startPos(inputStream.tellg())
{
    setg(d_buffer, nullptr, nullptr);
}


std::streambuf::pos_type encode_hex_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    return
            pos % sizeof(d_buffer) == 0
            && which == std::ios_base::in
            && (d_inputStream.clear(),
                d_inputStream.seekg(d_startPos + pos / 2))
        ? pos
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::int_type encode_hex_streambuf::underflow()
{
    char inputByte;
    if (d_inputStream.get(inputByte)) {
        std::ostrstream (d_buffer, sizeof(d_buffer)) << encode_hex(inputByte);
        setg(d_buffer, d_buffer, d_buffer + sizeof(d_buffer));
        return std::streambuf::traits_type::to_int_type(*d_buffer);
    } else return std::streambuf::traits_type::eof();
}

}  // close namespace cryptopals
