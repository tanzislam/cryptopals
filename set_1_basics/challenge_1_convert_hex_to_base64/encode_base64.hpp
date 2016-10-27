#ifndef INCLUDED_ENCODE_BASE64
#define INCLUDED_ENCODE_BASE64

#include "base64_common.hpp"
#include <ostream>
#include <streambuf>

namespace cryptopals {

class encode_base64
    : public base64_common
{
  public:
    typedef base64_common::binary_t input_t;
    encode_base64(const input_t & input)
        : d_input(input) { }

  private:
    const input_t & d_input;
    static char convertToBase64(uint8_t value);
    friend std::ostream & operator<<(std::ostream &, const encode_base64 &);
};


std::ostream & operator<<(std::ostream & output,
                          const encode_base64 & manipulator);


class encode_base64_streambuf
    : public std::streambuf
{
    std::ostream & d_outputStream;
    char d_buffer[3];

    void resetBuffer() { setp(d_buffer, d_buffer + sizeof(d_buffer)); }
    void invalidateBuffer() { setp(nullptr, nullptr); }
    void encode();

  protected:
    int_type overflow(int_type ch = std::char_traits<char>::eof());
    int sync();

  public:
    encode_base64_streambuf(std::ostream & outputStream);
    ~encode_base64_streambuf() { sync(); }
};

}  // close namespace cryptopals

#endif
