#ifndef INCLUDED_DETECT_AES_IN_ECB_MODE
#define INCLUDED_DETECT_AES_IN_ECB_MODE

#include <tuple>
#include <istream>

namespace cryptopals {

///@return Line number, starting offset in line, and repeat count
std::tuple<unsigned int, unsigned int, unsigned int> detect_aes_in_ecb_mode(
    std::istream & input);

}  // namespace cryptopals

#endif
