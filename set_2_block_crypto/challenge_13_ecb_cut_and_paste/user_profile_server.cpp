#include "user_profile_server.hpp"
#include "generate_random.hpp"
#include <aes.h>
#include <sstream>
#include "encode_user_profile.hpp"
#include "aes_ecb_encrypt.hpp"
#include "aes_ecb_decrypt.hpp"
#include <strstream>
#include "key_value_parser.hpp"

namespace cryptopals {

static auto key = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);

std::string generate_encrypted_user_profile(const std::string & email)
{
    std::istringstream encodedProfile(profile_for(email));
    std::ostringstream outputStream;
    aes_ecb_encrypt(outputStream, encodedProfile, key);
    return outputStream.str();
}


std::unordered_map<std::string, std::string> parse_encrypted_user_profile(
    const std::string & encrypted_user_profile)
{
    std::ostringstream decryptedProfile;
    aes_ecb_decrypt(decryptedProfile,
                    std::istrstream(encrypted_user_profile.c_str(),
                                    encrypted_user_profile.size())
                        .seekg(0),
                    key);
    return parse_structured_cookie(decryptedProfile.str());
}

}  // namespace cryptopals
