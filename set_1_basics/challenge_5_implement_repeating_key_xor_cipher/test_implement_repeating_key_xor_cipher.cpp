#include <gtest/gtest.h>
#include "repeating_stringstream.hpp"
#include <sstream>
#include "encode_hex.hpp"
#include "xor.hpp"


TEST(RepeatingStringStream, RepeatsStringAsStream)
{
    cryptopals::repeating_stringstream sstream;
    sstream << "helloworld";

    std::string extractedChars;
    sstream >> std::setw(7) >> extractedChars;
    EXPECT_EQ("hellowo", extractedChars);

    sstream >> std::setw(7) >> extractedChars;
    EXPECT_EQ("rldhell", extractedChars);
    EXPECT_TRUE(sstream);

    sstream >> std::setw(5) >> extractedChars;
    EXPECT_EQ("oworl", extractedChars);

    sstream << "!ExtensionTest";
    sstream >> std::setw(12) >> extractedChars;
    EXPECT_EQ("d!ExtensionT", extractedChars);

    sstream >> std::setw(10) >> extractedChars;
    EXPECT_EQ("esthellowo", extractedChars);
}


TEST(ImplementRepeatingKeyXorCipher, EncryptsProperly)
{
    std::istringstream input("Burning 'em, if you ain't quick and nimble\n"
                             "I go crazy when I hear a cymbal");

    cryptopals::repeating_stringstream xorKey;
    xorKey << "ICE";

    std::ostringstream hexOutputStream;
    cryptopals::encode_hex_streambuf hexEncoder(hexOutputStream);
    cryptopals::xor_streambuf xorCombiner(input, xorKey);
    std::istream(&xorCombiner).get(hexEncoder, '\0');
    EXPECT_EQ("0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a262263"
              "24272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b2028"
              "3165286326302e27282f",
              hexOutputStream.str());
}
