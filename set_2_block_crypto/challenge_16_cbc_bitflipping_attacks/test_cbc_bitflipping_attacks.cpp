#include <gtest/gtest.h>
#include "server.hpp"
#include "forge_admin_profile_cbc.hpp"

TEST(Server, EncryptsAndDecryptsAndConfirmsNotAdmin)
{
    using namespace cryptopals;
    EXPECT_FALSE(is_admin_cookie(generate_encrypted_cookie("hello world")));
    EXPECT_FALSE(is_admin_cookie(generate_encrypted_cookie("hi;admin=true;!")));
    EXPECT_FALSE(is_admin_cookie(generate_encrypted_cookie("hi&admin=true&!")));
}


TEST(ForgeAdminProfileCbc, CreatesForgedAdminProfile)
{
    const auto forged = cryptopals::forge_admin_profile_cbc();
    EXPECT_TRUE(cryptopals::is_admin_cookie(forged));
}
