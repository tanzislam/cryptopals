#include "encode_base64.hpp"
#include <cassert>
#include <ostream>

namespace cryptopals {


char encode_base64::convertToBase64(uint8_t value)
{
    static const uint8_t s_maxInputValue =
            (1 << encode_base64::s_numBitsInBase64Digit) - 1;
    assert(value <= s_maxInputValue);

    // Base64 conversion table is A-Z, then a-z, then 0-9, then '+' and '/'
    if (value <= 'Z' - 'A') return 'A' + value;
    value -= 'Z' - 'A' + 1;
    if (value <= 'z' - 'a') return 'a' + value;
    value -= 'z' - 'a' + 1;
    if (value <= '9' - '0') return '0' + value;
    value -= '9' - '0' + 1;
    return value ? '/' : '+';
}


std::ostream & operator<<(std::ostream & output,
                          const encode_base64 & manipulator)
{
    if (manipulator.d_input[0] == boost::none) {
        output.setstate(std::ios_base::failbit);
        return output;
    }

    // First base64 digit is composed of high 6 bits of first octet
    output <<
            encode_base64::convertToBase64(
                    *manipulator.d_input[0]
                    >> encode_base64::s_numBitsInOctet
                       - encode_base64::s_numBitsInBase64Digit
            );

    // Second base64 digit is composed of:
    //  - low 2 bits of first octet
    //  - high 4 bits of second octet
    static const size_t s_numRemainingBitsFromFirstOctet =
            encode_base64::s_numBitsInOctet
            - encode_base64::s_numBitsInBase64Digit;
    static const size_t s_numBitsFromSecondOctet =
            encode_base64::s_numBitsInBase64Digit
            - s_numRemainingBitsFromFirstOctet;
    uint8_t inputBitsForSecondBase64Digit =
            (
                    *manipulator.d_input[0]
                    & (1u << s_numRemainingBitsFromFirstOctet) - 1
            ) << s_numBitsFromSecondOctet;
    inputBitsForSecondBase64Digit +=
            manipulator.d_input[1].value_or(0)
            >> encode_base64::s_numBitsInOctet - s_numBitsFromSecondOctet;
    output << encode_base64::convertToBase64(inputBitsForSecondBase64Digit);

    // Third base64 digit is composed of:
    //  - low 4 bits of second octet
    //  - high 2 bits of third octet
    static const size_t s_numRemainingBitsFromSecondOctet =
            encode_base64::s_numBitsInOctet - s_numBitsFromSecondOctet;
    static const size_t s_numBitsFromThirdOctet =
            encode_base64::s_numBitsInBase64Digit
            - s_numRemainingBitsFromSecondOctet;
    try {
        uint8_t inputBitsForThirdBase64Digit =
                (
                        manipulator.d_input[1].value()
                        & (1u << s_numRemainingBitsFromSecondOctet) - 1
                ) << s_numBitsFromThirdOctet;
        inputBitsForThirdBase64Digit +=
                manipulator.d_input[2].value_or(0)
                >> encode_base64::s_numBitsInOctet - s_numBitsFromThirdOctet;
        output << encode_base64::convertToBase64(inputBitsForThirdBase64Digit);
    } catch (const boost::bad_optional_access &) {
        output << "==";
        return output;
    }

    // Fourth base64 digit is composed of low 6 bits of third octet
    static const size_t s_numRemainingBitsFromThirdOctet =
            encode_base64::s_numBitsInOctet - s_numBitsFromThirdOctet;
    try {
        uint8_t inputBitsForFourthBase64Digit =
                manipulator.d_input[2].value()
                & (1u << s_numRemainingBitsFromThirdOctet) - 1;
        output << encode_base64::convertToBase64(inputBitsForFourthBase64Digit);
    } catch (const boost::bad_optional_access &) {
        output << '=';
    }
    return output;
}

}  // close namespace cryptopals
