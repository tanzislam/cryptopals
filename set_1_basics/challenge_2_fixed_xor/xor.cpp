#include "xor.hpp"

namespace cryptopals {

xor_streambuf::xor_streambuf(std::istream & inputStream1,
                             std::istream & inputStream2)
    : d_inputStream1(inputStream1), d_inputStream2(inputStream2)
{
    setg(nullptr, nullptr, nullptr);
}


std::streambuf::int_type xor_streambuf::underflow()
{
    char input1, input2;
    if (d_inputStream1.get(input1))
        if (d_inputStream2.get(input2)) {
            d_buffer = char(input1 ^ input2);
            setg(&d_buffer, &d_buffer, &d_buffer + 1);
            return d_buffer;
        } else
            d_inputStream1.unget();
    return std::streambuf::underflow();
}

}  // close namespace cryptopals
