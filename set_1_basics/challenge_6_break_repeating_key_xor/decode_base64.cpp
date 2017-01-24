#include "decode_base64.hpp"
#include <stdexcept>
#include <istream>
#include <boost/io/ios_state.hpp>
#include <cstdlib>

namespace cryptopals {

uint8_t decode_base64::convertFromBase64(char base64Digit)
{
    if ('A' <= base64Digit && base64Digit <= 'Z') return base64Digit - 'A';
    uint8_t segmentStart = 'Z' - 'A' + 1;
    if ('a' <= base64Digit && base64Digit <= 'z')
        return segmentStart + base64Digit - 'a';
    segmentStart += 'z' - 'a' + 1;
    if ('0' <= base64Digit && base64Digit <= '9')
        return segmentStart + base64Digit - '0';
    if (base64Digit == '+') return 62;
    if (base64Digit == '/') return 63;
    throw std::domain_error("unrecognized Base64 digit as input");
}


std::istream & operator>>(std::istream & input,
                          const decode_base64 & manipulator)
{
    try {
        boost::io::ios_exception_saver exceptionsSaver(input);
        input.exceptions(std::istream::failbit);

        char base64Digit;
        input >> base64Digit;
        if (!input) input.setstate(std::ios_base::failbit);
        manipulator.d_output[0] = decode_base64::convertFromBase64(base64Digit)
                                  << (
                                        decode_base64::s_numBitsInOctet
                                        - decode_base64::s_numBitsInBase64Digit
                                     );

        input >> base64Digit;
        if (!input) input.setstate(std::ios_base::failbit);
        uint8_t value = decode_base64::convertFromBase64(base64Digit);
        static const size_t s_numRemainingBitsInFirstOctet =
                decode_base64::s_numBitsInOctet
                - decode_base64::s_numBitsInBase64Digit;
        static const size_t s_numBitsInSecondOctet =
                decode_base64::s_numBitsInBase64Digit
                - s_numRemainingBitsInFirstOctet;
        manipulator.d_output[0].get() += value >> s_numBitsInSecondOctet;
        manipulator.d_output[1] = (value & ((1u << s_numBitsInSecondOctet) - 1))
                                  << (
                                        decode_base64::s_numBitsInOctet
                                        - s_numBitsInSecondOctet
                                     );

        input >> base64Digit;
        if (!input) input.setstate(std::ios_base::failbit);
        if (base64Digit == '=') {
            if (manipulator.d_output[1].get() != 0)
                input.setstate(std::ios_base::failbit);
            manipulator.d_output[1].reset();
            input >> base64Digit;
            if (!input || base64Digit != '=')
                input.setstate(std::ios_base::failbit);
            return input;
        }

        value = decode_base64::convertFromBase64(base64Digit);
        static const size_t s_numRemainingBitsInSecondOctet =
                decode_base64::s_numBitsInOctet - s_numBitsInSecondOctet;
        static const size_t s_numBitsInThirdOctet =
                decode_base64::s_numBitsInBase64Digit
                - s_numRemainingBitsInSecondOctet;
        manipulator.d_output[1].get() += value >> s_numBitsInThirdOctet;
        manipulator.d_output[2] = (value & ((1u << s_numBitsInThirdOctet) - 1))
                                  << (
                                        decode_base64::s_numBitsInOctet
                                        - s_numBitsInThirdOctet
                                     );

        input >> base64Digit;
        if (!input) input.setstate(std::ios_base::failbit);
        if (base64Digit == '=') {
            if (manipulator.d_output[2].get() != 0)
                input.setstate(std::ios_base::failbit);
            manipulator.d_output[2].reset();
            return input;
        }
        manipulator.d_output[2].get() += decode_base64
                                            ::convertFromBase64(base64Digit);
    } catch (...) {
        input.setstate(std::ios_base::failbit);
    }
    return input;
}


decode_base64_streambuf::decode_base64_streambuf(std::istream & inputStream)
    : d_inputStream(inputStream), d_startPos(inputStream.tellg())
{
    setg(d_buffer, nullptr, nullptr);
}


std::streambuf::pos_type decode_base64_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    return
            pos % sizeof(d_buffer) == 0
            && which == std::ios_base::in
            && (d_inputStream.clear(),
                d_inputStream.seekg(d_startPos + pos * 4 / 3))
        ? pos
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type decode_base64_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    return
            abs(off) % sizeof(d_buffer) == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
            && -off <= (d_inputStream.tellg() - d_startPos) * 3 / 4
            && (d_inputStream.clear(),
                d_inputStream.seekg(off * 4 / 3, dir))
        ? std::streampos((d_inputStream.tellg() - d_startPos) * 3 / 4)
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type decode_base64_streambuf::underflow()
{
    decode_base64::output_t output;
    if (d_inputStream >> decode_base64(output)) {
        d_buffer[0] = output[0].get();
        if (!output[1]) {
            setg(d_buffer, d_buffer, d_buffer + 1);
            return reinterpret_cast<unsigned char &>(*d_buffer);
        }
        d_buffer[1] = output[1].get();
        if (!output[2]) {
            setg(d_buffer, d_buffer, d_buffer + 2);
            return reinterpret_cast<unsigned char &>(*d_buffer);
        }
        d_buffer[2] = output[2].get();
        setg(d_buffer, d_buffer, d_buffer + 3);
        return reinterpret_cast<unsigned char &>(*d_buffer);
    } else return std::streambuf::traits_type::eof();
}

}  // close namespace cryptopals
