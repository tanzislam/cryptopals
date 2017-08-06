#ifndef INCLUDED_DECODE_BASE64
#define INCLUDED_DECODE_BASE64

#include "base64_common.hpp"
#include <istream>
#include <streambuf>
#include "disable_method.hpp"

namespace cryptopals {

class decode_base64
    : public base64_common
{
  public:
    typedef base64_common::binary_t output_t;
    explicit decode_base64(output_t & output)
        : d_output(output) { }

  private:
    output_t & d_output;
    static uint8_t convertFromBase64(char base64Digit);
    friend std::istream & operator>>(std::istream & input,
                                     const decode_base64 & manipulator);
};


std::istream & operator>>(std::istream & input,
                          const decode_base64 & manipulator);


class decode_base64_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream;
    char d_buffer[3];
    std::istream::pos_type d_startPos;

  protected:
    virtual int_type underflow();
    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in);
    virtual pos_type seekoff(off_type off,
                             std::ios_base::seekdir dir,
                             std::ios_base::openmode which = std::ios_base::in);

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(int sync())
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type overflow())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    explicit decode_base64_streambuf(std::istream & inputStream);
};

}  // close namespace cryptopals

#endif
