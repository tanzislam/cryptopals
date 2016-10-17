#ifndef INCLUDED_XOR
#define INCLUDED_XOR

#include <streambuf>
#include <istream>

namespace cryptopals {

class xor_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream1;
    std::istream & d_inputStream2;
    char d_buffer;

  protected:
    int_type underflow();

  public:
    xor_streambuf(std::istream & inputStream1, std::istream & inputStream2);
};

}  // close namespace cryptopals

#endif
