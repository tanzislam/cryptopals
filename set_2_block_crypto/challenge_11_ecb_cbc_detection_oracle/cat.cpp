#include "cat.hpp"
#include <cassert>

namespace cryptopals {

cat_streambuf::cat_streambuf(std::istream & inputStream1,
                             std::istream & inputStream2)
    : d_inputStream1(inputStream1),
      d_inputStream2(inputStream2),
      d_inputBuffer()
{
    setg(nullptr, nullptr, nullptr);
}


std::streambuf::pos_type cat_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    assert(pos == std::streampos(0));
    assert(which == std::ios_base::in);
    return
            pos == std::streampos(0)
            && which == std::ios_base::in
            && (d_inputStream1.clear(), d_inputStream1.seekg(pos))
            && (d_inputStream2.clear(), d_inputStream2.seekg(pos))
        ? (setg(&d_inputBuffer, nullptr, nullptr), pos)
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type cat_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    assert(off == 0);
    assert(dir == std::ios_base::cur);
    assert(which == std::ios_base::in);
    return
            off == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
        ? d_inputStream1.rdbuf()->pubseekoff(0, dir, which)
                + (d_inputStream1.eof()
                        ? d_inputStream2.rdbuf()->pubseekoff(0, dir, which)
                        : std::streampos(0))
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type cat_streambuf::underflow()
{
    assert((!gptr() && !egptr()) || (gptr() && egptr() && gptr() == egptr()));
    if (!d_inputStream1.get(d_inputBuffer)
            && !d_inputStream2.get(d_inputBuffer))
        return std::streambuf::underflow();
    setg(&d_inputBuffer, &d_inputBuffer, &d_inputBuffer + 1);
    return std::streambuf::traits_type::to_int_type(d_inputBuffer);
}

}  // end namespace cryptopals
