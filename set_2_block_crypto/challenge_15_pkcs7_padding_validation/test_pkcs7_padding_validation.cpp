#include <gtest/gtest.h>
#include "pkcs7_unpad_wrap.hpp"

TEST(Pkcs7PaddingValidation, DetectsAndRemovesValidPadding)
{
    EXPECT_EQ("ICE ICE BABY",
              cryptopals::unpad_pkcs7("ICE ICE BABY\x04\x04\x04\x04"));
}


TEST(Pkcs7PaddingValidation, DetectsInvalidPadding)
{
    EXPECT_ANY_THROW(
        { cryptopals::unpad_pkcs7("ICE ICE BABY\x05\x05\x05\x05"); });
    EXPECT_ANY_THROW(
        { cryptopals::unpad_pkcs7("ICE ICE BABY\x01\x02\x03\x04"); });
}
