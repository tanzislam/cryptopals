#include "xor.hpp"

namespace cryptopals {

xor_streambuf::xor_streambuf(std::istream & inputStream1,
                             std::istream & inputStream2)
    : d_inputStream1(inputStream1),
      d_inputStream2(&inputStream2),
      d_outputStream(nullptr)
{
    setg(nullptr, nullptr, nullptr);
}


xor_streambuf::xor_streambuf(std::ostream & outputStream,
                             std::istream & otherInputStream)
    : d_inputStream1(otherInputStream),
      d_inputStream2(nullptr),
      d_outputStream(&outputStream)
{
    setp(nullptr, nullptr);
}


std::streambuf::int_type xor_streambuf::underflow()
{
    char input1, input2;
    if (d_inputStream1.get(input1)) {
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
    char otherInput;
    if (d_inputStream1.get(otherInput)) {
        if (*d_outputStream << char(ch ^ otherInput)) return 1;
        else d_inputStream1.unget();
    }
    return traits_type::eof();
}

}  // close namespace cryptopals
