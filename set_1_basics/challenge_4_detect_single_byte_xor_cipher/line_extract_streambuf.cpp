#include "line_extract_streambuf.hpp"
#include <cassert>
#include <cstdlib>

namespace cryptopals {

line_extract_streambuf::line_extract_streambuf(std::istream & inputStream)
        : d_inputStream(inputStream),
          d_buffer(),
          d_startPos(inputStream.tellg()),
          d_numEndBytesRead(0)
{
    if (!d_inputStream || d_startPos == std::streampos(-1))
        throw std::ios_base::failure("Could not obtain initial position");
    setg(nullptr, nullptr, nullptr);
}


std::streambuf::pos_type line_extract_streambuf::seekpos(
    std::streambuf::pos_type pos,
    std::ios_base::openmode which)
{
    assert(pos == std::streampos(0));
    assert(which == std::ios_base::in);
    return pos == std::streampos(0) && which == std::ios_base::in
                   && (d_inputStream.clear(), d_inputStream.seekg(d_startPos))
               ? (d_numEndBytesRead = 0, pos)
               : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type line_extract_streambuf::seekoff(
    std::streambuf::off_type off,
    std::ios_base::seekdir dir,
    std::ios_base::openmode which)
{
    assert(off == 0);
    assert(dir == std::ios_base::cur);
    assert(which == std::ios_base::in);
    return off == 0 && dir == std::ios_base::cur && which == std::ios_base::in
                   && (d_inputStream.clear(), true)
               ? std::streampos(d_inputStream.tellg() - d_startPos
                                - d_numEndBytesRead)
               : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type line_extract_streambuf::underflow()
{
    assert((!gptr() && !egptr()) || (gptr() && egptr() && gptr() == egptr()));
    if (d_numEndBytesRead) return std::streambuf::traits_type::eof();

    try {
        if (d_inputStream.get(d_buffer) && d_buffer != '\n') {
            if (d_buffer == '\r') {
                if (d_inputStream.peek() == '\n') {
                    ++d_numEndBytesRead;
                    d_inputStream.ignore();
                }
                ++d_numEndBytesRead;
                return std::streambuf::traits_type::eof();
            }
            setg(&d_buffer, &d_buffer, &d_buffer + 1);
            return std::streambuf::traits_type::to_int_type(d_buffer);
        } else {
            !d_inputStream.fail() && ++d_numEndBytesRead;
            return std::streambuf::traits_type::eof();
        }
    } catch (...) {
        return std::streambuf::traits_type::eof();
    }
}

}  // namespace cryptopals
