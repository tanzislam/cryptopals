#include "line_extract_streambuf.hpp"

namespace cryptopals {

line_extract_streambuf::line_extract_streambuf(std::istream & inputStream)
    : d_inputStream(inputStream), d_startPos(inputStream.tellg())
{
    if (!d_inputStream || d_startPos == std::streampos(-1))
        throw std::ios_base::failure("Could not obtain initial position");
    setg(&d_buffer, nullptr, nullptr);
}


std::streambuf::pos_type line_extract_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    return
            pos == std::streampos(0)
            && which == std::ios_base::in
            && (d_inputStream.clear(), d_inputStream.seekg(d_startPos))
        ? pos
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type line_extract_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    return
            off % sizeof(d_buffer) == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
            && -off <= d_inputStream.tellg() - d_startPos
            && (d_inputStream.clear(), d_inputStream.seekg(off, dir))
        ? std::streampos(d_inputStream.tellg() - d_startPos)
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type line_extract_streambuf::underflow()
{
    try {
        if (d_inputStream.get(d_buffer) && d_buffer != '\n') {
            if (d_buffer == '\r') {
                if (d_inputStream.peek() == '\n') d_inputStream.ignore();
                return std::streambuf::traits_type::eof();
            }
            setg(&d_buffer, &d_buffer, &d_buffer + 1);
            return std::streambuf::traits_type::to_int_type(d_buffer);
        } else return std::streambuf::traits_type::eof();
    } catch (...) {
        return std::streambuf::traits_type::eof();
    }
}

}  // close namespace cryptopals
