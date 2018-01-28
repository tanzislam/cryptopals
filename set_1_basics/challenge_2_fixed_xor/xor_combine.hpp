#ifndef INCLUDED_XOR_COMBINE
#define INCLUDED_XOR_COMBINE

#include <iosfwd>

namespace cryptopals {

void xor_combine(std::ostream & hexEncodedOutputStream,
                 std::istream & hexEncodedInputStream1,
                 std::istream & hexEncodedInputStream2);

}  // namespace cryptopals

#endif
