#include <gtest/gtest.h>
#include "cbc_padding_oracle.hpp"
#include "attack_cbc_padding_oracle.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "decode_base64.hpp"
#include <iostream>

TEST(CbcPaddingOracle, GeneratesDissimilarCiphertextsAndIvs)
{
    auto prevCiphertextAndIv = cryptopals::generate_encrypted_message_and_iv();
    for (auto i = 0; i < 10; ++i) {
        auto ciphertextAndIv = cryptopals::generate_encrypted_message_and_iv();
        EXPECT_NE(ciphertextAndIv.first, prevCiphertextAndIv.first);
        EXPECT_NE(ciphertextAndIv.second, prevCiphertextAndIv.second);
        prevCiphertextAndIv = ciphertextAndIv;
    }
}


TEST(CbcPaddingOracle, VerifiesPaddingCorrectly)
{
    auto ciphertextAndIv = cryptopals::generate_encrypted_message_and_iv();
    EXPECT_TRUE(cryptopals::is_padding_valid(ciphertextAndIv.first,
                                             ciphertextAndIv.second));
    EXPECT_FALSE(
        cryptopals::is_padding_valid("The quick brown ", "0123456789ABCDEF"));
}


TEST(AttackCbcPaddingOracle, RecoversPlaintext)
{
    for (auto i = 0; i < 10; ++i) {
        auto ciphertextAndIv = cryptopals::generate_encrypted_message_and_iv();
        EXPECT_NO_THROW({
            auto msg = cryptopals::attack_cbc_padding_oracle(
                &cryptopals::is_padding_valid,
                ciphertextAndIv.first,
                ciphertextAndIv.second);
            boost::iostreams::stream<boost::iostreams::array_source>
                msgStream(msg.data(), msg.size());
            cryptopals::decode_base64_streambuf base64Decoder(msgStream);
            std::cout << &base64Decoder << '\n';
        });
    }
}
