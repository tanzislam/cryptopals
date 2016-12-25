#ifndef INCLUDED_XOR
#define INCLUDED_XOR

#include <streambuf>
#include <istream>

namespace cryptopals {

class xor_streambuf
    : public std::streambuf
{
    std::istream & d_inputStream1;
    std::istream * d_inputStream2;
    char d_inputBuffer;
    std::ostream * d_outputStream;

  protected:
    int_type underflow();
    int_type overflow(int_type ch = traits_type::eof());

  public:
    /// For use with a std::istream wrapper (i.e. in "pull mode")
    xor_streambuf(std::istream & inputStream1, std::istream & inputStream2);

    /// For use with a std::ostream wrapper (i.e. in "push mode")
    xor_streambuf(std::ostream & outputStream, std::istream & otherInputStream);
};

}  // close namespace cryptopals

#endif
