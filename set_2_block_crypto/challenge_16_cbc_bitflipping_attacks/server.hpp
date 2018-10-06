#ifndef INCLUDED_SERVER
#define INCLUDED_SERVER

#include <string>

namespace cryptopals {

std::string generate_encrypted_cookie(const std::string & data);

bool is_admin_cookie(const std::string & encryptedCookie);

}  // namespace cryptopals

#endif
