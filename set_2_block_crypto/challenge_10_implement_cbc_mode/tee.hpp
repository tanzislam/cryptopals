#ifndef INCLUDED_TEE
#define INCLUDED_TEE

#include <streambuf>
#include <istream>
#include <ostream>

namespace cryptopals {

class tee_streambuf
    : public std::streambuf
{
    std::istream * d_inputStream;
    std::ostream & d_teeStream, * d_teeStream2;
    char d_inputBuffer;

  protected:
    pos_type seekoff(std::streambuf::off_type off,
                     std::ios_base::seekdir dir,
                     std::ios_base::openmode which);
    int_type underflow();
    int_type overflow(int_type ch = traits_type::eof());

  public:
    tee_streambuf(std::istream & inputStream, std::ostream & teeStream);
    tee_streambuf(std::ostream & teeStream1, std::ostream & teeStream2);
};

}  // close namespace cryptopals

#endif
