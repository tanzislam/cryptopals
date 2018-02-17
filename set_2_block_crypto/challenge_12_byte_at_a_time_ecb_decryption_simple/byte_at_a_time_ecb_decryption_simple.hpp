#ifndef INCLUDED_BYTE_AT_A_TIME_ECB_DECRYPTION_SIMPLE
#define INCLUDED_BYTE_AT_A_TIME_ECB_DECRYPTION_SIMPLE

#include <ostream>
#include <istream>
#include <string>

namespace cryptopals {

typedef void (*EcbOracle)(std::ostream &, std::istream &);

unsigned int find_block_size(EcbOracle ecbOracle);

char find_suffix_byte(EcbOracle ecbOracle,
                      const std::string & knownSuffix,
                      unsigned int blockSize);

std::string find_suffix(EcbOracle ecbOracle, unsigned int blockSize);

}  // namespace cryptopals

#endif
