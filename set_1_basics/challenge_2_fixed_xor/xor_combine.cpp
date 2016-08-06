#include "xor_combine.hpp"
#include "decode_hex.hpp"
#include <boost/io/ios_state.hpp>
#include <istream>
#include <ostream>
#include <iomanip>

namespace cryptopals {

void xor_combine(std::ostream & outputStream,
                 std::istream & inputStream1,
                 std::istream & inputStream2)
{
    boost::io::ios_flags_saver formatGuard(outputStream);
    decode_hex::decoded_t value1, value2;
    outputStream << std::hex;
    while (
            inputStream1 >> decode_hex(value1)
            && inputStream2 >> decode_hex(value2)
    ) {
        outputStream << (value1 ^ value2);
    }
}

}  // close namespace cryptopals
