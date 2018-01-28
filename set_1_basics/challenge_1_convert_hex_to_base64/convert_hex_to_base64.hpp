#ifndef INCLUDED_CONVERT_HEX_TO_BASE64
#define INCLUDED_CONVERT_HEX_TO_BASE64

#include <iosfwd>

namespace cryptopals {

void convert_hex_to_base64(std::ostream & output, std::istream & input);

}  // namespace cryptopals

#endif
