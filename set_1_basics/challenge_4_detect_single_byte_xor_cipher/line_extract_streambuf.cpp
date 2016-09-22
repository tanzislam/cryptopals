#include "line_extract_streambuf.hpp"

namespace cryptopals {

line_extract_streambuf::line_extract_streambuf(std::istream & inputStream)
    : d_inputStream(inputStream), d_startPos(inputStream.tellg())
{
    setg(&d_buffer, nullptr, nullptr);
}


std::streambuf::pos_type line_extract_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    return
            pos == 0
            && which == std::ios_base::in
            && (d_inputStream.clear(), d_inputStream.seekg(d_startPos))
        ? pos
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::int_type line_extract_streambuf::underflow()
{
    if (d_inputStream.get(d_buffer) && d_buffer != '\n') {
        setg(&d_buffer, &d_buffer, &d_buffer + 1);
        return std::streambuf::traits_type::to_int_type(d_buffer);
    } else return std::streambuf::traits_type::eof();
}

}  // close namespace cryptopals
