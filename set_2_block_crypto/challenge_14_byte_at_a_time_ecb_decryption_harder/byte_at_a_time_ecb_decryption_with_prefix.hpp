#ifndef INCLUDED_BYTE_AT_A_TIME_ECB_DECRYPTION_WITH_PREFIX
#define INCLUDED_BYTE_AT_A_TIME_ECB_DECRYPTION_WITH_PREFIX

#include <ostream>
#include <istream>
#include <utility>
#include <string>

namespace cryptopals {

typedef void (*EcbOracle)(std::ostream &, std::istream &);

std::pair<unsigned int, unsigned int> find_block_size_and_prefix_length(
    EcbOracle ecbOracle,
    const unsigned int minBlockSize = 3u,
    const unsigned int tolerance = 3u,
    const unsigned int stability = 3u);

char find_suffix_byte_with_prefix(EcbOracle ecbOracle,
                                  const std::string & knownSuffix,
                                  unsigned int blockSize,
                                  unsigned int prefixLength);

std::string find_suffix_with_prefix(EcbOracle ecbOracle,
                                    unsigned int blockSize,
                                    unsigned int prefixLength);

}  // namespace cryptopals

#endif
