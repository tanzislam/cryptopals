#ifndef INCLUDED_DETECT_ENCRYPTION_METHOD
#define INCLUDED_DETECT_ENCRYPTION_METHOD

#include <ostream>
#include <istream>

namespace cryptopals {

enum struct EncryptionMode { ECB, CBC };

EncryptionMode detectEncryptionMode(void (*blackBox)(std::ostream &,
                                                     std::istream &));

}  // namespace cryptopals

#endif
