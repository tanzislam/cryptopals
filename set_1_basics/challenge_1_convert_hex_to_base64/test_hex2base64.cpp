#include "decode_hex.hpp"
#include "encode_base64.hpp"
#include "convert_hex_to_base64.hpp"
#include <gtest/gtest.h>
#include <sstream>


TEST(DecodeHex, DeocdesUnit)
{
    std::istringstream inputStream(std::string("FF"));
    cryptopals::decode_hex::decoded_t result;
    inputStream >> cryptopals::decode_hex(result);
    EXPECT_EQ(0xFFu, result);
    EXPECT_FALSE(inputStream.fail());
}


TEST(DecodeHex, DiscardsIncompleteInput)
{
    std::istringstream inputStream(std::string("A"));
    cryptopals::decode_hex::decoded_t result = 0;
    inputStream >> cryptopals::decode_hex(result);
    EXPECT_EQ(0u, result);
    EXPECT_TRUE(inputStream.fail());
}


TEST(EncodeBase64, EncodesOneByte)
{
    std::ostringstream outputStream;
    outputStream << cryptopals::encode_base64({'M'});
    EXPECT_EQ("TQ==", outputStream.str());
    EXPECT_FALSE(outputStream.fail());
}


TEST(EncodeBase64, EncodesTwoBytes)
{
    std::ostringstream outputStream;
    outputStream << cryptopals::encode_base64({'M', 'a'});
    EXPECT_EQ("TWE=", outputStream.str());
    EXPECT_FALSE(outputStream.fail());
}


TEST(EncodeBase64, EncodesThreeBytes)
{
    std::ostringstream outputStream;
    outputStream << cryptopals::encode_base64({'M', 'a', 'n'});
    EXPECT_EQ("TWFu", outputStream.str());
    EXPECT_FALSE(outputStream.fail());
}


TEST(EncodeBase64, DiscardsIncompleteInput)
{
    std::ostringstream outputStream;
    outputStream << cryptopals::encode_base64({ });
    EXPECT_EQ(0u, outputStream.str().length());
    EXPECT_TRUE(outputStream.fail());
}


TEST(ConvertHexToBase64, ConvertsHexToBase64)
{
    std::ostringstream outputStream;
    std::istringstream inputStream("49276d206b696c6c696e6720796f757220627261696"
                                   "e206c696b65206120706f69736f6e6f7573206d7573"
                                   "68726f6f6d");
    cryptopals::convert_hex_to_base64(outputStream, inputStream);
    EXPECT_EQ(
            "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t",
            outputStream.str()
    );
    EXPECT_FALSE(outputStream.fail());
}


TEST(ConvertHexToBase64, ConvertsHexToBase64Partial)
{
    std::ostringstream outputStream;
    std::istringstream inputStream("4D");
    cryptopals::convert_hex_to_base64(outputStream, inputStream);
    EXPECT_EQ("TQ==", outputStream.str());
    EXPECT_FALSE(outputStream.fail());
}


TEST(ConvertHexToBase64, ConvertsHexToBase64Partial2)
{
    std::ostringstream outputStream;
    std::istringstream inputStream("4D61");
    cryptopals::convert_hex_to_base64(outputStream, inputStream);
    EXPECT_EQ("TWE=", outputStream.str());
    EXPECT_FALSE(outputStream.fail());
}
