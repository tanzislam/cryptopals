#ifndef INCLUDED_SKIP_CHARS
#define INCLUDED_SKIP_CHARS

#include <streambuf>
#include <istream>

namespace cryptopals {

class skip_chars_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream;
    char d_buffer;
    unsigned int d_skipLength;

  protected:
    virtual int_type underflow();

  public:
    skip_chars_streambuf(std::istream & inputStream,
                         unsigned int startPos,
                         unsigned int skipLength);
};

}  // close namespace cryptopals

#endif
