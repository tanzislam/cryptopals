#include "xor.hpp"
#include "encode_hex.hpp"
#include "xor_combine.hpp"
#include <gtest/gtest.h>
#include <sstream>


TEST(Xor, ComputesXorOfUnitAsInput)
{
    std::istringstream inputStream1("\x1c");
    std::istringstream inputStream2("\x68");

    cryptopals::xor_streambuf xorStreambuf(inputStream1, inputStream2);
    std::istream xorStream(&xorStreambuf);
    EXPECT_EQ(0, xorStream.tellg());

    std::string output;
    xorStream >> output;
    EXPECT_EQ("\x74", output);

    xorStream.clear();
    EXPECT_EQ(1, xorStream.tellg());
}


TEST(Xor, ComputesXorOfUnitAsOutput)
{
    std::istringstream inputStream("\x1c");
    std::ostringstream outputStream;

    cryptopals::xor_streambuf xorStreambuf(outputStream, inputStream);
    std::ostream xorStream(&xorStreambuf);
    EXPECT_EQ(0, xorStream.tellp());

    xorStream << "\x68";
    EXPECT_EQ("\x74", outputStream.str());
    EXPECT_EQ(1, xorStream.tellp());
}


TEST(EncodeHex, EncodesHexOfUnit)
{
    std::ostringstream outputStream;
    cryptopals::encode_hex_streambuf hexEncoder(outputStream);
    std::istringstream("\xfe\x30\xa2").get(hexEncoder);
    EXPECT_EQ("fe30a2", outputStream.str());
}


TEST(XorCombine, HexDecodesInputsAndAppliesXor)
{
    std::istringstream inputStream1("1c0111001f010100061a024b53535009181c"),
        inputStream2("686974207468652062756c6c277320657965");
    std::ostringstream outputStream;
    cryptopals::xor_combine(outputStream, inputStream1, inputStream2);
    EXPECT_EQ("746865206b696420646f6e277420706c6179", outputStream.str());
}
