#ifndef INCLUDED_ATTACK_CBC_PADDING_ORACLE
#define INCLUDED_ATTACK_CBC_PADDING_ORACLE

#include <string>

namespace cryptopals {

std::string attack_cbc_padding_oracle(bool (*oracle)(const std::string &,
                                                     const std::string &),
                                      const std::string & ciphertext,
                                      const std::string & initVector);

}  // namespace cryptopals

#endif
