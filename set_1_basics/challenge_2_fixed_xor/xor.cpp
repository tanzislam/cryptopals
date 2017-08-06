#include "xor.hpp"
#include <cassert>

namespace cryptopals {

xor_streambuf::xor_streambuf(std::istream & inputStream1,
                             std::istream & inputStream2)
    : d_inputStream1(inputStream1),
      d_inputStream2(&inputStream2),
      d_inputBuffer(),
      d_outputStream(nullptr)
{
    setg(nullptr, nullptr, nullptr);
}


xor_streambuf::xor_streambuf(std::ostream & outputStream,
                             std::istream & otherInputStream)
    : d_inputStream1(otherInputStream),
      d_inputStream2(nullptr),
      d_inputBuffer(),
      d_outputStream(&outputStream)
{
    setp(nullptr, nullptr);
}


std::streambuf::int_type xor_streambuf::underflow()
{
    assert(!d_outputStream);
    assert((!gptr() && !egptr()) || (gptr() && egptr() && gptr() == egptr()));
    char input1;
    if (d_inputStream1.get(input1)) {
        char input2;
        if (d_inputStream2->get(input2)) {
            d_inputBuffer = char(input1 ^ input2);
            setg(&d_inputBuffer, &d_inputBuffer, &d_inputBuffer + 1);
            return d_inputBuffer;
        } else
            d_inputStream1.unget();
    }
    return std::streambuf::underflow();
}


std::streambuf::int_type xor_streambuf::overflow(std::streambuf::int_type ch)
{
    assert(d_outputStream);
    assert((!pptr() && !epptr()) || (pptr() && epptr() && pptr() == epptr()));
    char otherInput;
    if (d_inputStream1.get(otherInput)) {
        if (*d_outputStream << char(ch ^ otherInput)) return 1;
        else d_inputStream1.unget();
    }
    return traits_type::eof();
}


std::streambuf::pos_type xor_streambuf::seekoff(off_type off,
                                                std::ios_base::seekdir dir,
                                                std::ios_base::openmode which)
{
    assert(off == 0);
    assert(dir == std::ios_base::cur);
    assert(d_inputStream2
            ? which == std::ios_base::in
            : which == std::ios_base::out);
    return which == std::ios_base::in
            ? d_inputStream2->tellg()
            : d_outputStream->tellp();
}

}  // close namespace cryptopals
