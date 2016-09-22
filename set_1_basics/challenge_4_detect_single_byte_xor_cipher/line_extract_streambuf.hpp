#ifndef INCLUDED_LINE_EXTRACT_STREAMBUF
#define INCLUDED_LINE_EXTRACT_STREAMBUF

#include <streambuf>
#include <istream>

namespace cryptopals {

class line_extract_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream;
    char d_buffer;
    std::istream::pos_type d_startPos;

  protected:
    virtual int_type underflow();
    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in);

  public:
    line_extract_streambuf(std::istream & inputStream);
};

}  // close namespace cryptopals

#endif
