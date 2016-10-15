#ifndef INCLUDED_DECODE_HEX
#define INCLUDED_DECODE_HEX

#include <istream>
#include <boost/integer.hpp>
#include <streambuf>

namespace cryptopals {

class decode_hex
{
    static const unsigned int s_numBitsInHexDigit = 4;
    static const unsigned int s_numBitsInOctet = 8;
    static const unsigned int s_numHexDigitsInOctet =
            s_numBitsInOctet / s_numBitsInHexDigit;

  public:
    typedef boost::uint_t<s_numBitsInOctet>::fast decoded_t;

    decode_hex(decoded_t & value)
        : d_value(value) { }

  private:
    decoded_t & d_value;
    friend std::istream & operator>>(std::istream &, const decode_hex &);
    friend class decode_hex_streambuf;
};


std::istream & operator>>(std::istream & input, const decode_hex & manipulator);


class decode_hex_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream;
    decode_hex::decoded_t d_buffer;
    std::istream::pos_type d_startPos;

  protected:
    virtual int_type underflow();
    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in);

  public:
    decode_hex_streambuf(std::istream & inputStream);
};

}  // close namespace cryptopals

#endif
