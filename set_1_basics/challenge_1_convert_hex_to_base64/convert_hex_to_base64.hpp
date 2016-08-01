#ifndef CONVERT_HEX_TO_BASE64
#define CONVERT_HEX_TO_BASE64

#include <iosfwd>

namespace cryptopals {

void convert_hex_to_base64(std::ostream & output, std::istream & input);

}  // close namespace cryptopals

#endif
