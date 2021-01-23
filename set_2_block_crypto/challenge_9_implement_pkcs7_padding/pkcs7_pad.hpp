#ifndef INCLUDED_PKCS7_PAD
#define INCLUDED_PKCS7_PAD

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include "disable_method.hpp"

namespace cryptopals {

class pkcs7_pad_streambuf : public std::streambuf, private boost::noncopyable
{
    std::istream * d_inputStream;
    char d_buffer;
    char * d_paddingBuffer;
    unsigned int d_inputLength, d_blockSize;

    pkcs7_pad_streambuf(pkcs7_pad_streambuf &) = delete;
    pkcs7_pad_streambuf & operator=(pkcs7_pad_streambuf &) = delete;

  protected:
    virtual int_type underflow();

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
    pkcs7_pad_streambuf(std::istream & inputStream, unsigned int blockSize);
    ~pkcs7_pad_streambuf();
};

}  // namespace cryptopals

#endif
