#ifndef INCLUDED_ENCODE_HEX
#define INCLUDED_ENCODE_HEX

#include <cstdint>
#include <ostream>
#include <streambuf>
#include "disable_method.hpp"

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

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(pos_type seekoff(off_type,
                                    std::ios_base::seekdir,
                                    std::ios_base::openmode))
    DISABLE_METHOD(pos_type seekpos(pos_type, std::ios_base::openmode))
    DISABLE_METHOD(int sync())
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type underflow())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    encode_hex_streambuf(std::ostream & outputStream);
};

}  // close namespace cryptopals

#endif
