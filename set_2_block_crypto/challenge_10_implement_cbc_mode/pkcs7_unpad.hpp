#ifndef INCLUDED_PKCS7_UNPAD
#define INCLUDED_PKCS7_UNPAD

#include <streambuf>
#include <istream>
#include <ostream>

namespace cryptopals {

class pkcs7_unpad_streambuf
    : public std::streambuf
{
    char * d_currentBlock;
    const unsigned int d_blockSize;
    std::istream * d_inputStream;
    std::ostream * d_outputStream;

    void resetInputBlock();
    void resetOutputBlock();
    char * findFirstPaddingByte() const;

  protected:
    virtual int_type underflow();
    virtual int_type overflow(int_type ch = traits_type::eof());
    virtual int sync();

    /// only supports rewinding (i.e. pos == 0 and which == in)
    virtual pos_type seekpos(pos_type pos,
                             std::ios_base::openmode which = std::ios_base::in);

    /// only supports querying current position
    /// (i.e. off == 0, dir == cur, which == in)
    virtual pos_type seekoff(off_type off,
                             std::ios_base::seekdir dir,
                             std::ios_base::openmode which = std::ios_base::in);

  public:
    pkcs7_unpad_streambuf(std::istream & inputStream, unsigned int blockSize);
    pkcs7_unpad_streambuf(std::ostream & outputStream, unsigned int blockSize);
    ~pkcs7_unpad_streambuf();
};

}  // close namespace cryptopals

#endif
