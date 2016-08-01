#ifndef XOR_COMBINE
#define XOR_COMBINE

#include <iosfwd>

namespace cryptopals {

void xor_combine(std::ostream & outputStream,
                 std::istream & inputStream1,
                 std::istream & inputStream2);

}  // close namespace cryptopals

#endif