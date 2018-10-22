#ifndef INCLUDED_CBC_PADDING_ORACLE
#define INCLUDED_CBC_PADDING_ORACLE

#include <utility>
#include <string>

namespace cryptopals {

std::pair<std::string, std::string> generate_encrypted_message_and_iv();

bool is_padding_valid(const std::string & ciphertext, const std::string & iv);

}  // namespace cryptopals

#endif
