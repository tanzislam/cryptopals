#ifndef INCLUDED_ENCODE_HEX
#define INCLUDED_ENCODE_HEX

#include <cstdint>
#include <ostream>
#include <streambuf>

namespace cryptopals {

class encode_hex
{
    const uint8_t & d_value;
    friend std::ostream & operator<<(std::ostream &, const encode_hex &);

  public:
    encode_hex(const uint8_t & value)
        : d_value(value) { }
};


std::ostream & operator<<(std::ostream & output,
                          const encode_hex & manipulator);


class encode_hex_streambuf
    : public std::streambuf
{
    std::ostream & d_outputStream;

  protected:
    std::streambuf::int_type overflow(std::streambuf::int_type value);

  public:
    encode_hex_streambuf(std::ostream & outputStream);
};

}  // close namespace cryptopals

#endif
