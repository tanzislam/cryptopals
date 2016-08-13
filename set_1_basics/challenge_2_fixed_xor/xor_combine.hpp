#ifndef INCLUDED_XOR_COMBINE
#define INCLUDED_XOR_COMBINE

#include <iosfwd>

namespace cryptopals {

void xor_combine(std::ostream & outputStream,
                 std::istream & hexEncodedInputStream1,
                 std::istream & hexEncodedInputStream2,
                 bool hexEncodeOutput = true);

}  // close namespace cryptopals

#endif
