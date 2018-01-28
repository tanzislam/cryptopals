#include <gtest/gtest.h>
#include "generate_random.hpp"
#include <aes.h>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include "cat.hpp"
#include "detect_encryption_method.hpp"
#include "encryption_oracle.hpp"

TEST(GenerateRandom, GeneratesRandomWithoutRepetition)
{
    const std::string & random =
        cryptopals::generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
    boost::io::ios_flags_saver flagsSaver(std::cout, std::ios_base::hex);
    std::unordered_set<unsigned char> values;
    std::for_each(random.begin(), random.end(), [&values](unsigned char c) {
        std::cout << ' ' << int(c);
        values.insert(c);
    });
    std::cout << std::endl;
    EXPECT_LT(random.size() - values.size(), 3);
}


TEST(CatStreambuf, ConcatenatesStreamsAndRewinds)
{
    std::istringstream str1("hello"), str2("there"), str3("foo"), str4("bar");

    cryptopals::cat_streambuf cat12(str1, str2);
    std::istream str1_and_2(&cat12);

    cryptopals::cat_streambuf cat123(str1_and_2, str3);
    std::istream str1_and_2_and_3(&cat123);

    cryptopals::cat_streambuf cat1234(str1_and_2_and_3, str4);
    std::istream str1_and_2_and_3_and_4(&cat1234);

    std::string concatenatedString;
    std::getline(str1_and_2_and_3_and_4, concatenatedString);
    EXPECT_EQ(concatenatedString, "hellotherefoobar");

    str1_and_2_and_3_and_4.clear();
    EXPECT_EQ(16, str1_and_2_and_3_and_4.tellg());

    str1_and_2_and_3_and_4.seekg(0);
    concatenatedString.clear();
    std::getline(str1_and_2_and_3_and_4, concatenatedString);
    EXPECT_EQ(concatenatedString, "hellotherefoobar");
}


namespace cryptopals {
extern EncryptionMode lastEncryptionMode;
}  // namespace cryptopals

TEST(EncryptionOracle, DetectsEncryptionMethod)
{
    cryptopals::EncryptionMode encryptionMode;
    for (size_t i = 0; i < 50; ++i) {
        encryptionMode =
            cryptopals::detectEncryptionMode(cryptopals::encryptionOracle);
        EXPECT_EQ(encryptionMode, cryptopals::lastEncryptionMode);
    }
}
