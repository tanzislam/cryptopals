#ifndef ENCODE_BASE64
#define ENCODE_BASE64

#include <iosfwd>
#include <boost/math/common_factor_ct.hpp>
#include <array>
#include <boost/optional.hpp>

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

}  // close namespace cryptopals

#endif
