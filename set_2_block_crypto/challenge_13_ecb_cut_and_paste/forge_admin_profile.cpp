#include "forge_admin_profile.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "pkcs7_pad.hpp"
#include <aes.h>
#include <boost/range/size.hpp>
#include <iterator>

namespace cryptopals {

std::string forge_admin_profile(
    std::string (*userProfileServer)(const std::string &),
    const std::string & email)
{
    // Encrypting: email=          admin...........&uid=10&role=user............
    //             |               |               |               |
    // (Here, a "." is a PKCS#7 padding character.)
    static const char admin[] = "admin";
    boost::iostreams::stream<boost::iostreams::array_source>
        targetRole(admin, sizeof admin - 1);
    pkcs7_pad_streambuf padder(targetRole, CryptoPP::AES::BLOCKSIZE);
    const auto encryptedSample = userProfileServer(
        std::string(CryptoPP::AES::BLOCKSIZE - boost::size("email=") + 1, ' ')
        + std::string(std::istreambuf_iterator<char>(&padder), {}));

    // Elongate email such that the trailing "user" ends up in its own block.
    // So encrypting: email=foo@bar.com  &uid=10&role=user............
    //                |               |               |               |
    const auto adjustedEmail =
        email
        + std::string(CryptoPP::AES::BLOCKSIZE
                          - (boost::size("email=") - 1 + email.size()
                             + boost::size("&uid=10&role=") - 1)
                                % CryptoPP::AES::BLOCKSIZE,
                      ' ');
    auto roleProfile = userProfileServer(adjustedEmail);

    // Replace the trailing "user............" block with "admin...........".
    return roleProfile.replace(roleProfile.size() - CryptoPP::AES::BLOCKSIZE,
                               CryptoPP::AES::BLOCKSIZE,
                               encryptedSample,
                               CryptoPP::AES::BLOCKSIZE,
                               CryptoPP::AES::BLOCKSIZE);
}

}  // namespace cryptopals
