#include "tee.hpp"
#include <cassert>

namespace cryptopals {

tee_streambuf::tee_streambuf(std::istream & inputStream,
                             std::ostream & teeStream)
    : d_inputStream(&inputStream), d_teeStream(teeStream), d_teeStream2(nullptr)
{
    setg(nullptr, nullptr, nullptr);
}


std::streambuf::pos_type tee_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    assert(d_inputStream);
    return
            off == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
        ? std::streampos(d_inputStream->tellg())
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type tee_streambuf::underflow()
{
    assert(d_inputStream);
    char input;
    if (d_inputStream->get(input)) {
        if (d_teeStream.put(input)) {
            d_inputBuffer = input;
            setg(&d_inputBuffer, &d_inputBuffer, &d_inputBuffer + 1);
            return static_cast<unsigned char>(d_inputBuffer);
        } else d_inputStream->unget();
    }
    return std::streambuf::underflow();
}


tee_streambuf::tee_streambuf(std::ostream & teeStream1,
                             std::ostream & teeStream2)
    : d_inputStream(nullptr), d_teeStream(teeStream1), d_teeStream2(&teeStream2)
{
    setp(nullptr, nullptr);
}


std::streambuf::int_type tee_streambuf::overflow(std::streambuf::int_type ch)
{
    assert(d_teeStream2);
    if (ch != std::streambuf::traits_type::eof())
        d_teeStream.put(ch) && d_teeStream2->put(ch);
    return d_teeStream && *d_teeStream2
                ? !std::streambuf::traits_type::eof()
                : std::streambuf::overflow(ch);
}

}  // close namespace cryptopals
