#ifndef INCLUDED_XOR
#define INCLUDED_XOR

#include <streambuf>
#include <istream>
#include <ostream>
#include "disable_method.hpp"

namespace cryptopals {

class xor_streambuf : public std::streambuf
{
    std::istream & d_inputStream1;
    std::istream * d_inputStream2;
    char d_inputBuffer;
    std::ostream * d_outputStream;

  protected:
    int_type underflow();
    int_type overflow(int_type ch = traits_type::eof());
    pos_type seekoff(off_type off,
                     std::ios_base::seekdir dir,
                     std::ios_base::openmode which = std::ios_base::in);
    DEFINE_DEFAULT_SYNC_METHOD(d_outputStream)

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(pos_type seekpos(pos_type, std::ios_base::openmode))
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    /// For use with a std::istream wrapper (i.e. in "pull mode")
    xor_streambuf(std::istream & inputStream1, std::istream & inputStream2);

    /// For use with a std::ostream wrapper (i.e. in "push mode")
    xor_streambuf(std::ostream & outputStream, std::istream & otherInputStream);
};

}  // namespace cryptopals

#endif
