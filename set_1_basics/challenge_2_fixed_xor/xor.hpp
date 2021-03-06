#ifndef INCLUDED_XOR
#define INCLUDED_XOR

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include <ostream>
#include "disable_method.hpp"

namespace cryptopals {

class xor_streambuf : public std::streambuf, private boost::noncopyable
{
    std::istream & d_inputStream1;
    std::istream * d_inputStream2;
    char d_inputBuffer;
    std::ostream * d_outputStream;

  protected:
    int_type underflow() override;
    int_type overflow(int_type ch = traits_type::eof()) override;
    pos_type seekoff(
        off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which = std::ios_base::in) override;
    DEFINE_DEFAULT_SYNC_METHOD(d_outputStream)

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DEFINE_DEFAULT_SEEKPOS_METHOD
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
