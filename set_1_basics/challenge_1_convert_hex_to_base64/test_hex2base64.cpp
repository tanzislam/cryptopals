#include <gtest/gtest.h>
#include <sstream>
#include "decode_hex.hpp"
#include <string>
#include "encode_base64.hpp"
#include "convert_hex_to_base64.hpp"


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


TEST(DecodeHexStreamBuf, ComputesPositionsAndOffsets)
{
    std::istringstream inputStream("5468697320697320612063696365206461792e");
    inputStream.ignore(4);
    EXPECT_EQ(4, inputStream.tellg());

    cryptopals::decode_hex_streambuf hexDecoder(inputStream);
    EXPECT_EQ(0,
              hexDecoder.pubseekoff(0, std::ios_base::cur, std::ios_base::in));

    std::istream hexDecodedInput(&hexDecoder);
    EXPECT_EQ(0, hexDecodedInput.tellg());
    EXPECT_EQ(4, inputStream.tellg());

    std::string word;
    std::getline(hexDecodedInput, word, 'a');
    EXPECT_EQ("is is ", word);
    EXPECT_EQ(7, hexDecodedInput.tellg());
    EXPECT_EQ(18, inputStream.tellg());

    EXPECT_TRUE(hexDecodedInput.seekg(12));
    EXPECT_TRUE(inputStream);
    EXPECT_EQ(12, hexDecodedInput.tellg());
    EXPECT_EQ(28, inputStream.tellg());

    EXPECT_TRUE(hexDecodedInput.seekg(-5, std::ios_base::cur));
    EXPECT_TRUE(inputStream);
    EXPECT_EQ(7, hexDecodedInput.tellg());
    EXPECT_EQ(18, inputStream.tellg());

    EXPECT_FALSE(hexDecodedInput.seekg(-3, std::ios_base::end));
    EXPECT_TRUE(inputStream);
    EXPECT_EQ(18, inputStream.tellg());
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
