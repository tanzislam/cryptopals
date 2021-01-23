#ifndef INCLUDED_PKCS7_UNPAD
#define INCLUDED_PKCS7_UNPAD

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include <ostream>
#include "disable_method.hpp"

namespace cryptopals {

class pkcs7_unpad_streambuf : public std::streambuf, private boost::noncopyable
{
    char * d_currentBlock;
    const unsigned int d_blockSize;
    std::istream * d_inputStream;
    std::ostream * d_outputStream;

    void resetInputBlock();
    void resetOutputBlock();
    char * findFirstPaddingByte() const;
    void emitBuffer(char * end);

  protected:
    virtual int_type underflow() override;
    virtual int_type overflow(int_type ch = traits_type::eof()) override;
    DEFINE_DEFAULT_SYNC_METHOD(d_outputStream)

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
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    pkcs7_unpad_streambuf(std::istream & inputStream, unsigned int blockSize);
    pkcs7_unpad_streambuf(std::ostream & outputStream, unsigned int blockSize);
    ~pkcs7_unpad_streambuf();
};

}  // namespace cryptopals

#endif
