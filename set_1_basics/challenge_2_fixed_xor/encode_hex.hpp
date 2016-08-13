#ifndef INCLUDED_ENCODE_HEX
#define INCLUDED_ENCODE_HEX

#include <cstdint>
#include <iosfwd>

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

}  // close namespace cryptopals

#endif
