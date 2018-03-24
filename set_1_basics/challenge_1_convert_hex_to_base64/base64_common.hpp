#ifndef INCLUDED_BASE64_COMMON
#define INCLUDED_BASE64_COMMON

#include <boost/integer/common_factor_ct.hpp>
#include <array>
#include <boost/optional.hpp>

namespace cryptopals {

struct base64_common {
  protected:
    static const size_t s_numBitsInBase64Digit = 6;
    static const size_t s_numBitsInOctet = 8;

    static const size_t s_numBitsPerOperation =
        boost::integer::static_lcm<s_numBitsInBase64Digit,
                                   s_numBitsInOctet>::value;
    static const size_t s_numOctetsPerOperation = s_numBitsPerOperation
                                                  / s_numBitsInOctet;

    typedef std::array<boost::optional<uint8_t>, s_numOctetsPerOperation>
        binary_t;
};

}  // namespace cryptopals

#endif
