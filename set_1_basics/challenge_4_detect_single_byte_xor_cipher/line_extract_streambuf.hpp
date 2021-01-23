#ifndef INCLUDED_LINE_EXTRACT_STREAMBUF
#define INCLUDED_LINE_EXTRACT_STREAMBUF

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include "disable_method.hpp"

namespace cryptopals {

class line_extract_streambuf : public std::streambuf, private boost::noncopyable
{
    std::istream & d_inputStream;
    char d_buffer;
    std::istream::pos_type d_startPos;
    unsigned int d_numEndBytesRead;

  protected:
    virtual int_type underflow() override;

    /// only supports rewinding (i.e. pos == 0 and which == in)
    virtual pos_type seekpos(
        pos_type pos,
        std::ios_base::openmode which = std::ios_base::in) override;

    /// only supports querying current position
    /// (i.e. off == 0, dir == cur, which == in)
    virtual pos_type seekoff(
        off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which = std::ios_base::in) override;

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(int sync())
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type overflow(int_type))
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    explicit line_extract_streambuf(std::istream & inputStream);
};

}  // namespace cryptopals

#endif
