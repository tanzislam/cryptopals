#ifndef INCLUDED_KEY_VALUE_PARSER
#define INCLUDED_KEY_VALUE_PARSER

#include <unordered_map>
#include <string>

namespace cryptopals {

std::unordered_map<std::string, std::string> parse_structured_cookie(
    const std::string & cookie);

}  // namespace cryptopals

#endif
