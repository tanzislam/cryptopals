#ifndef INCLUDED_USER_PROFILE_SERVER
#define INCLUDED_USER_PROFILE_SERVER

#include <string>
#include <unordered_map>

namespace cryptopals {

std::string generate_encrypted_user_profile(const std::string & email);

std::unordered_map<std::string, std::string> parse_encrypted_user_profile(
    const std::string & encrypted_user_profile);

}  // namespace cryptopals

#endif
