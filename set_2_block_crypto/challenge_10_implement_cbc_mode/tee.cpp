#include "tee.hpp"
#include <cassert>

namespace cryptopals {

tee_streambuf::tee_streambuf(std::istream & inputStream,
                             std::ostream & teeStream)
        : d_inputStream(&inputStream),
          d_teeStream(teeStream),
          d_teeStream2(nullptr),
          d_inputBuffer()
{
    setg(nullptr, nullptr, nullptr);
}


std::streambuf::pos_type tee_streambuf::seekoff(std::streambuf::off_type off,
                                                std::ios_base::seekdir dir,
                                                std::ios_base::openmode which)
{
    assert(d_inputStream);
    assert(off == 0);
    assert(dir == std::ios_base::cur);
    assert(which == std::ios_base::in);
    return off == 0 && dir == std::ios_base::cur && which == std::ios_base::in
               ? std::streampos(d_inputStream->tellg())
               : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type tee_streambuf::underflow()
{
    assert(d_inputStream);
    assert((!gptr() && !egptr()) || (gptr() && egptr() && gptr() == egptr()));
    char input;
    if (d_inputStream->get(input)) {
        if (d_teeStream.put(input)) {
            d_inputBuffer = input;
            setg(&d_inputBuffer, &d_inputBuffer, &d_inputBuffer + 1);
            return std::streambuf::traits_type::to_int_type(d_inputBuffer);
        } else
            d_inputStream->unget();
    }
    return std::streambuf::underflow();
}


tee_streambuf::tee_streambuf(std::ostream & teeStream1,
                             std::ostream & teeStream2)
        : d_inputStream(nullptr),
          d_teeStream(teeStream1),
          d_teeStream2(&teeStream2),
          d_inputBuffer()
{
    setp(nullptr, nullptr);
}


std::streambuf::int_type tee_streambuf::overflow(std::streambuf::int_type ch)
{
    assert(d_teeStream2);
    assert((!pptr() && !epptr()) || (pptr() && epptr() && pptr() == epptr()));
    if (ch != std::streambuf::traits_type::eof())
        d_teeStream.put(ch) && d_teeStream2->put(ch);
    return d_teeStream && *d_teeStream2 ? !std::streambuf::traits_type::eof()
                                        : std::streambuf::overflow(ch);
}


int tee_streambuf::sync()
{
    assert(d_teeStream2);
    try {
        return d_teeStream.flush() && d_teeStream2->flush() ? 0 : -1;
    } catch (...) {
        return -1;
    }
}

}  // namespace cryptopals
