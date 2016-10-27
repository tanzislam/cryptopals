#include "skip_chars.hpp"

namespace cryptopals {

skip_chars_streambuf::skip_chars_streambuf(std::istream & inputStream,
                                           unsigned int startPos,
                                           unsigned int skipLength)
    : d_inputStream(inputStream), d_skipLength(skipLength)
{
    if (!d_inputStream.ignore(startPos))
        throw std::ios_base::failure("Failed to ignore leading characters");
    setg(&d_buffer, nullptr, nullptr);
}


std::streambuf::int_type skip_chars_streambuf::underflow()
{
    try {
        if (d_inputStream.get(d_buffer)) {
            setg(&d_buffer, &d_buffer, &d_buffer + 1);
            d_inputStream.ignore(d_skipLength - 1);
            return std::streambuf::traits_type::to_int_type(d_buffer);
        } else return std::streambuf::traits_type::eof();
    } catch (...) {
        return std::streambuf::traits_type::eof();
    }
}

}  // close namespace cryptopals
