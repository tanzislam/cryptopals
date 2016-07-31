#ifndef DECODE_HEX
#define DECODE_HEX

#include <iosfwd>
#include <boost/integer.hpp>

namespace cryptopals {

class decode_hex
{
    static const size_t s_numBitsInHexDigit = 4;
    static const size_t s_numBitsInOctet = 8;
    static const size_t s_numHexDigitsInOctet =
            s_numBitsInOctet / s_numBitsInHexDigit;

  public:
    typedef boost::uint_t<s_numBitsInOctet>::fast decoded_t;

    decode_hex(decoded_t & value)
        : d_value(value) { }

  private:
    decoded_t & d_value;
    friend std::istream & operator>>(std::istream &, const decode_hex &);
};


std::istream & operator>>(std::istream & input, const decode_hex & manipulator);

}  // close namespace cryptopals

#endif