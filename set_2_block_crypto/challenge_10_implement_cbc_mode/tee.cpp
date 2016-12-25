#include "tee.hpp"

namespace cryptopals {

tee_streambuf::tee_streambuf(std::istream & inputStream,
                             std::ostream & teeStream)
    : d_inputStream(inputStream), d_teeStream(teeStream)
{
    setg(nullptr, nullptr, nullptr);
}


std::streambuf::pos_type tee_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    return
            off == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
        ? std::streampos(d_inputStream.tellg())
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type tee_streambuf::underflow()
{
    char input;
    bool getSucceeded = false, putSucceeded = false;
    if (d_inputStream.get(input) && (getSucceeded = true))
        if (d_teeStream.put(input) && (putSucceeded = true)) {
            d_inputBuffer = input;
            setg(&d_inputBuffer, &d_inputBuffer, &d_inputBuffer + 1);
            return static_cast<unsigned char>(d_inputBuffer);
        } else d_inputStream.unget();
    return std::streambuf::underflow();
}

}  // close namespace cryptopals
