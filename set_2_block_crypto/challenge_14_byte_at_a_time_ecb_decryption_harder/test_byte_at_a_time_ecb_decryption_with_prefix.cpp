#include <gtest/gtest.h>
#include <aes.h>
#include "detect_encryption_method.hpp"
#include "ecb_encryption_oracle_with_prefix.hpp"
#include "byte_at_a_time_ecb_decryption_with_prefix.hpp"
#include <iostream>
#include "decode_base64.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <cstring>

TEST(EcbEncryptionOracleWithPrefix, EncryptsUsingEcb)
{
    EXPECT_EQ(cryptopals::EncryptionMode::ECB,
              cryptopals::detectEncryptionMode(cryptopals::ecbEncryptOracle2));
}


const auto blockSizeAndPrefixLength =
    cryptopals::find_block_size_and_prefix_length(
        cryptopals::ecbEncryptOracle2);


TEST(EcbEncryptionOracleWithPrefix, HasAesBlockSize)
{
    EXPECT_EQ(CryptoPP::AES::BLOCKSIZE, blockSizeAndPrefixLength.first);
    EXPECT_TRUE(5 <= blockSizeAndPrefixLength.second
                && blockSizeAndPrefixLength.second <= 15);
    std::cout << "Prefix length: " << blockSizeAndPrefixLength.second << '\n';
}


namespace cryptopals {
extern const char * const base64EncodedSuffix2;
}  // namespace cryptopals

TEST(EcbEncryptionOracleWithPrefix, FindsSuffixByte)
{
    cryptopals::decode_base64::output_t output;
    boost::iostreams::stream<boost::iostreams::array_source>(
        cryptopals::base64EncodedSuffix2,
        std::strlen(cryptopals::base64EncodedSuffix2))
            .seekg(0)
        >> cryptopals::decode_base64(output);

    auto suffixByte = cryptopals::find_suffix_byte_with_prefix(
        cryptopals::ecbEncryptOracle2,
        std::string(),
        blockSizeAndPrefixLength.first,
        blockSizeAndPrefixLength.second);
    EXPECT_EQ(output[0].get(), suffixByte);
    std::cout << "Found Suffix Byte: " << suffixByte << '\n';
}


TEST(EcbEncryptionOracleWithPrefix, FindsSuffix)
{
    boost::iostreams::stream<boost::iostreams::array_source>
        encodedSuffix(cryptopals::base64EncodedSuffix2,
                      std::strlen(cryptopals::base64EncodedSuffix2));
    std::string output;
    cryptopals::decode_base64_streambuf decoder(encodedSuffix);
    std::getline(std::istream(&decoder), output, '\0');
    auto suffix =
        cryptopals::find_suffix_with_prefix(cryptopals::ecbEncryptOracle2,
                                            blockSizeAndPrefixLength.first,
                                            blockSizeAndPrefixLength.second);
    EXPECT_EQ(output, suffix);
    std::cout << "Found Suffix:\n" << suffix << '\n';
}
