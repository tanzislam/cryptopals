#include "encode_user_profile.hpp"

namespace cryptopals {

std::string profile_for(const std::string & email)
{
    auto strippedEmail = email;
    std::size_t pos = 0;
    while ((pos = strippedEmail.find_first_of("&=", pos)) != std::string::npos)
        strippedEmail.erase(pos, 1);
    return "email=" + strippedEmail + "&uid=10&role=user";
}

}  // namespace cryptopals
