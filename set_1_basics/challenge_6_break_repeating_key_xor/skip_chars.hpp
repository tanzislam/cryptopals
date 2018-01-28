#ifndef INCLUDED_SKIP_CHARS
#define INCLUDED_SKIP_CHARS

#include <streambuf>
#include <istream>
#include "disable_method.hpp"

namespace cryptopals {

class skip_chars_streambuf : public std::streambuf
{
    std::istream & d_inputStream;
    char d_buffer;
    unsigned int d_skipLength;

  protected:
    virtual int_type underflow();

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(pos_type seekoff(off_type,
                                    std::ios_base::seekdir,
                                    std::ios_base::openmode))
    DISABLE_METHOD(pos_type seekpos(pos_type, std::ios_base::openmode))
    DISABLE_METHOD(int sync())
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type overflow())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    skip_chars_streambuf(std::istream & inputStream,
                         unsigned int startPos,
                         unsigned int skipLength);
};

}  // namespace cryptopals

#endif
