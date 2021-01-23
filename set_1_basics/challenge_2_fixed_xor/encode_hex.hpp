#ifndef INCLUDED_ENCODE_HEX
#define INCLUDED_ENCODE_HEX

#include <cstdint>
#include <ostream>
#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include "disable_method.hpp"

namespace cryptopals {

class encode_hex
{
    const uint8_t & d_value;
    friend std::ostream & operator<<(std::ostream &, const encode_hex &);

  public:
    explicit encode_hex(const uint8_t & value) : d_value(value) {}
};


std::ostream & operator<<(std::ostream & output,
                          const encode_hex & manipulator);


class encode_hex_streambuf : public std::streambuf, private boost::noncopyable
{
    std::ostream & d_outputStream;

  protected:
    std::streambuf::int_type overflow(std::streambuf::int_type value) override;
    DEFINE_DEFAULT_SYNC_METHOD(&d_outputStream)

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(pos_type seekoff(off_type,
                                    std::ios_base::seekdir,
                                    std::ios_base::openmode))
    DISABLE_METHOD(pos_type seekpos(pos_type, std::ios_base::openmode))
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type underflow())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    explicit encode_hex_streambuf(std::ostream & outputStream);
};

}  // namespace cryptopals

#endif
