#include "xor_combine.hpp"
#include <gtest/gtest.h>
#include <sstream>


TEST(XorCombine, HexDecodesInputsAndAppliesXor)
{
    std::istringstream inputStream1("1c0111001f010100061a024b53535009181c"),
                       inputStream2("686974207468652062756c6c277320657965");
    std::ostringstream outputStream;
    cryptopals::xor_combine(outputStream, inputStream1, inputStream2);
    EXPECT_EQ("746865206b696420646f6e277420706c6179", outputStream.str());
}