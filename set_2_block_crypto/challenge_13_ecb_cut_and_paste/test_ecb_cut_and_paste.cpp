#include <gtest/gtest.h>
#include "key_value_parser.hpp"
#include "encode_user_profile.hpp"
#include "user_profile_server.hpp"
#include <aes.h>
#include "forge_admin_profile.hpp"

TEST(KeyValueParser, ParsesStructuredCookie)
{
    std::string cookie = "foo=bar&baz=qux&zap=zazzle";
    const auto map = cryptopals::parse_structured_cookie(cookie);
    ASSERT_NE(map.find("foo"), map.end());
    EXPECT_EQ(map.at("foo"), "bar");
    ASSERT_NE(map.find("baz"), map.end());
    EXPECT_EQ(map.at("baz"), "qux");
    ASSERT_NE(map.find("zap"), map.end());
    EXPECT_EQ(map.at("zap"), "zazzle");
}


TEST(EncodeUserProfile, GeneratesEncodedUserProfile)
{
    EXPECT_EQ(cryptopals::profile_for("foo@bar.com"),
              "email=foo@bar.com&uid=10&role=user");
    EXPECT_EQ(cryptopals::profile_for("tom&jerry!=friends@example.com"),
              "email=tomjerry!friends@example.com&uid=10&role=user");
}


TEST(UserProfileServer, EncryptsAndDecryptsUserProfile)
{
    const std::string email = "foo@bar.com";
    const auto profile = cryptopals::generate_encrypted_user_profile(email);
    EXPECT_EQ(profile.size(),
              CryptoPP::AES::BLOCKSIZE
                  * (1
                     + ("email=" + email + "&uid=10&role=user").size()
                           / CryptoPP::AES::BLOCKSIZE));

    const auto decrypted = cryptopals::parse_encrypted_user_profile(profile);
    ASSERT_NE(decrypted.find("email"), decrypted.end());
    EXPECT_EQ(decrypted.at("email"), email);
    ASSERT_NE(decrypted.find("uid"), decrypted.end());
    EXPECT_EQ(decrypted.at("uid"), "10");
    ASSERT_NE(decrypted.find("role"), decrypted.end());
    EXPECT_EQ(decrypted.at("role"), "user");
}


TEST(ForgeAdminProfile, CreatesForgedAdminUserProfile)
{
    const std::string email = "foo@bar.com";
    const auto forged = cryptopals::forge_admin_profile(
        cryptopals::generate_encrypted_user_profile, email);
    EXPECT_EQ(forged.size(),
              CryptoPP::AES::BLOCKSIZE
                  * (1
                     + ("email=" + email + "&uid=10&role=admin").size()
                           / CryptoPP::AES::BLOCKSIZE));

    const auto decrypted = cryptopals::parse_encrypted_user_profile(forged);
    ASSERT_NE(decrypted.find("email"), decrypted.end());
    EXPECT_EQ(decrypted.at("email").compare(0, email.size(), email), 0);
    ASSERT_NE(decrypted.find("uid"), decrypted.end());
    EXPECT_EQ(decrypted.at("uid"), "10");
    ASSERT_NE(decrypted.find("role"), decrypted.end());
    EXPECT_EQ(decrypted.at("role"), "admin");
}
