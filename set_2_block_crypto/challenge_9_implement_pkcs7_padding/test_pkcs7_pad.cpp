#include <gtest/gtest.h>
#include <sstream>
#include "pkcs7_pad.hpp"
#include <string>
#include <iomanip>


TEST(Pkcs7Pad, PadsProperly)
{
    std::stringstream input;
    input << "YELLOW SUBMARINE";
    cryptopals::pkcs7_pad_streambuf pkcs7Padder(input, 20);
    std::string output;
    std::getline(std::istream(&pkcs7Padder).seekg(0), output, '\0');
    EXPECT_EQ(output, "YELLOW SUBMARINE\x04\x04\x04\x04");
}


TEST(Pkcs7Pad, HandlesNoPaddingNeeded)
{
    std::stringstream input;
    input << "YELLOW SUBMARINE !!!";
    cryptopals::pkcs7_pad_streambuf pkcs7Padder(input, 20);
    std::string output;
    std::getline(std::istream(&pkcs7Padder).seekg(0), output, '\0');
    EXPECT_EQ(output,
              "YELLOW SUBMARINE !!!"
              "\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14"
              "\x14\x14\x14\x14\x14\x14\x14\x14\x14\x14");
}


TEST(Pkcs7Pad, ReportsPosition)
{
    std::stringstream input;
    input << "YELLOW SUBMARINE";
    cryptopals::pkcs7_pad_streambuf pkcs7Padder(input, 20);
    std::istream inputStream(&pkcs7Padder);
    std::string output;

    EXPECT_EQ(0, inputStream.tellg());

    inputStream >> std::setw(4) >> output;
    EXPECT_EQ("YELL", output);
    EXPECT_EQ(4, inputStream.tellg());

    inputStream >> output;
    EXPECT_EQ("OW", output);
    EXPECT_EQ(6, inputStream.tellg());

    inputStream >> std::setw(9) >> output;
    EXPECT_EQ("SUBMARINE", output);
    EXPECT_EQ(16, inputStream.tellg());

    EXPECT_EQ('\x04', inputStream.get());
    EXPECT_EQ(17, inputStream.tellg());

    inputStream >> std::setw(3) >> output;
    EXPECT_EQ("\x04\x04\x04", output);

    inputStream.clear();
    EXPECT_EQ(20, inputStream.tellg());

    inputStream.seekg(0);
    EXPECT_EQ(0, inputStream.tellg());
}
