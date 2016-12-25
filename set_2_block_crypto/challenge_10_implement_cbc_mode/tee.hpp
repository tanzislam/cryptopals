#ifndef INCLUDED_TEE
#define INCLUDED_TEE

#include <streambuf>
#include <istream>
#include <ostream>

namespace cryptopals {

class tee_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream;
    std::ostream & d_teeStream;
    char d_inputBuffer;

  protected:
    pos_type seekoff(std::streambuf::off_type off,
                     std::ios_base::seekdir dir,
                     std::ios_base::openmode which);
    int_type underflow();

  public:
    tee_streambuf(std::istream & inputStream, std::ostream & teeStream);
};

}  // close namespace cryptopals

#endif
