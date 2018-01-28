#ifndef INCLUDED_CAT
#define INCLUDED_CAT

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include "disable_method.hpp"

namespace cryptopals {

class cat_streambuf : public std::streambuf, private boost::noncopyable
{
    std::istream & d_inputStream1;
    std::istream & d_inputStream2;
    char d_inputBuffer;

  protected:
    int_type underflow();

    /// only supports rewinding (i.e. pos == 0 and which == in)
    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in);

    /// only supports querying current position
    /// (i.e. off == 0, dir == cur, which == in)
    virtual pos_type seekoff(off_type off,
                             std::ios_base::seekdir dir,
                             std::ios_base::openmode which = std::ios_base::in);

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(int sync())
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type overflow(int_type))
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    cat_streambuf(std::istream & inputStream1, std::istream & inputStream2);
};

}  // namespace cryptopals

#endif
