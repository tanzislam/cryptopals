#include <gtest/gtest.h>
#include <aes.h>
#include "byte_at_a_time_ecb_decryption_simple.hpp"
#include "ecb_encryption_oracle.hpp"
#include "detect_encryption_method.hpp"
#include "decode_base64.hpp"
#include <strstream>
#include <iostream>

TEST(EcbEncryptionOracle, HasAesBlockSize)
{
    EXPECT_EQ(CryptoPP::AES::BLOCKSIZE,
              cryptopals::find_block_size(cryptopals::ecbEncryptOracle));
}


TEST(EcbEncryptionOracle, EncryptsUsingEcb)
{
    EXPECT_EQ(cryptopals::EncryptionMode::ECB,
              cryptopals::detectEncryptionMode(cryptopals::ecbEncryptOracle));
}


namespace cryptopals {
extern const char * const base64EncodedSuffix;
}  // namespace cryptopals

TEST(EcbEncryptionOracle, FindsSuffixByte)
{
    cryptopals::decode_base64::output_t output;
    std::istrstream(cryptopals::base64EncodedSuffix).seekg(0)
        >> cryptopals::decode_base64(output);
    auto suffixByte = cryptopals::find_suffix_byte(cryptopals::ecbEncryptOracle,
                                                   std::string(),
                                                   CryptoPP::AES::BLOCKSIZE);
    EXPECT_EQ(output[0].get(), suffixByte);
    std::cout << "Found Suffix Byte: " << suffixByte << '\n';
}


TEST(EcbEncryptionOracle, FindsSuffix)
{
    std::istrstream encodedSuffix(cryptopals::base64EncodedSuffix);
    std::string output;
    cryptopals::decode_base64_streambuf decoder(encodedSuffix);
    std::getline(std::istream(&decoder), output, '\0');
    auto suffix = cryptopals::find_suffix(cryptopals::ecbEncryptOracle,
                                          CryptoPP::AES::BLOCKSIZE);
    EXPECT_EQ(output, suffix);
    std::cout << "Found Suffix:\n" << suffix << '\n';
}
