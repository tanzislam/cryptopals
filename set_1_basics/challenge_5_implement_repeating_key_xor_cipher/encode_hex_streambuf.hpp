#ifndef INCLUDED_ENCODE_HEX_STREAMBUF
#define INCLUDED_ENCODE_HEX_STREAMBUF

#include <streambuf>
#include <istream>

namespace cryptopals {

class encode_hex_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream;
    char d_buffer[2];
    std::istream::pos_type d_startPos;

  protected:
    virtual int_type underflow();
    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in);

  public:
    encode_hex_streambuf(std::istream & inputStream);
};

}  // close namespace cryptopals

#endif
