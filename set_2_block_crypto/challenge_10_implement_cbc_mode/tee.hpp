#ifndef INCLUDED_TEE
#define INCLUDED_TEE

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include <ostream>
#include "disable_method.hpp"

namespace cryptopals {

class tee_streambuf : public std::streambuf, private boost::noncopyable
{
    std::istream * d_inputStream;
    std::ostream & d_teeStream;
    std::ostream * d_teeStream2;
    char d_inputBuffer;

  protected:
    pos_type seekoff(std::streambuf::off_type off,
                     std::ios_base::seekdir dir,
                     std::ios_base::openmode which);
    int_type underflow();
    int_type overflow(int_type ch = traits_type::eof());
    int sync();

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DEFINE_DEFAULT_SEEKPOS_METHOD
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    tee_streambuf(std::istream & inputStream, std::ostream & teeStream);
    tee_streambuf(std::ostream & teeStream1, std::ostream & teeStream2);
};

}  // namespace cryptopals

#endif
