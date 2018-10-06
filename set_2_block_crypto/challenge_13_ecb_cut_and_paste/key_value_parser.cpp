#include "key_value_parser.hpp"

namespace cryptopals {

std::unordered_map<std::string, std::string> parse_structured_cookie(
    const std::string & cookie, char separator)
{
    std::unordered_map<std::string, std::string> keyValuePairs;
    for (std::size_t pos = 0; pos != std::string::npos;) {
        auto endOfKey = cookie.find('=', pos);
        auto endOfValue = cookie.find(separator, endOfKey + 1);
        keyValuePairs[cookie.substr(pos, endOfKey - pos)] =
            cookie.substr(endOfKey + 1, endOfValue - endOfKey - 1);
        pos = endOfValue == std::string::npos ? endOfValue : endOfValue + 1;
    }
    return keyValuePairs;
}

}  // namespace cryptopals
