#ifndef INCLUDED_ENCODE_BASE64
#define INCLUDED_ENCODE_BASE64

#include <boost/math/common_factor_ct.hpp>
#include <array>
#include <boost/optional.hpp>
#include <ostream>
#include <streambuf>

namespace cryptopals {

class encode_base64
{
    static const size_t s_numBitsInBase64Digit = 6;
    static const size_t s_numBitsInOctet = 8;

    static const size_t s_numBitsPerOperation = boost::math::static_lcm<
                                                        s_numBitsInBase64Digit,
                                                        s_numBitsInOctet
                                                >::value;
    static const size_t s_numOctetsPerOperation =
            s_numBitsPerOperation / s_numBitsInOctet;

  public:
    typedef std::array<boost::optional<uint8_t>,
                       s_numOctetsPerOperation> input_t;

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
