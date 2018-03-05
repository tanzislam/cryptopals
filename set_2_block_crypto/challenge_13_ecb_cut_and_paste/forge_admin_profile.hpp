#ifndef INCLUDED_FORGE_ADMIN_PROFILE
#define INCLUDED_FORGE_ADMIN_PROFILE

#include <string>

namespace cryptopals {

// The userProfileServer must be generating encrypted versions of:
// email=<EMAIL>&uid=10&role=user
// Also assumed: ECB mode of operation, 16-byte block size
std::string forge_admin_profile(
    std::string (*userProfileServer)(const std::string &),
    const std::string & email);

}  // namespace cryptopals

#endif
