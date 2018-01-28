#include <gtest/gtest.h>
#include "hamming_distance.hpp"
#include <sstream>
#include "decode_base64.hpp"
#include <string>
#include "skip_chars.hpp"
#include <boost/asio.hpp>
#include <strstream>
#include "break_repeating_key_xor_cipher.hpp"
#include <iostream>


TEST(HammingDistance, CalculatesHammingDistanceCorrectly)
{
    EXPECT_EQ(37,
              cryptopals::hamming_distance("this is a test", "wokka wokka!!!"));
}


TEST(DecodeBase64, DecodesOneByte)
{
    std::istringstream inputStream("TQ==");
    cryptopals::decode_base64::output_t output;
    inputStream >> cryptopals::decode_base64(output);
    EXPECT_FALSE(inputStream.fail());
    ASSERT_TRUE(output[0]);
    EXPECT_EQ('M', output[0].get());
    EXPECT_FALSE(output[1]);
    EXPECT_FALSE(output[2]);
}


TEST(DecodeBase64, DecodesOneByteBoundaryCase)
{
    std::istringstream inputStream("TA==");
    cryptopals::decode_base64::output_t output;
    inputStream >> cryptopals::decode_base64(output);
    EXPECT_FALSE(inputStream.fail());
    ASSERT_TRUE(output[0]);
    EXPECT_EQ('L', output[0].get());
    EXPECT_FALSE(output[1]);
    EXPECT_FALSE(output[2]);
}


TEST(DecodeBase64, DecodesTwoBytes)
{
    std::istringstream inputStream("TWE=");
    cryptopals::decode_base64::output_t output;
    inputStream >> cryptopals::decode_base64(output);
    EXPECT_FALSE(inputStream.fail());
    ASSERT_TRUE(output[0]);
    EXPECT_EQ('M', output[0].get());
    ASSERT_TRUE(output[1]);
    EXPECT_EQ('a', output[1].get());
    EXPECT_FALSE(output[2]);
}


TEST(DecodeBase64, DecodesTwoBytesBoundaryCase)
{
    std::istringstream inputStream("bWA=");
    cryptopals::decode_base64::output_t output;
    inputStream >> cryptopals::decode_base64(output);
    EXPECT_FALSE(inputStream.fail());
    ASSERT_TRUE(output[0]);
    EXPECT_EQ('m', output[0].get());
    ASSERT_TRUE(output[1]);
    EXPECT_EQ('`', output[1].get());
    EXPECT_FALSE(output[2]);
}


TEST(DecodeBase64, DecodesThreeBytes)
{
    std::istringstream inputStream("TWFu");
    cryptopals::decode_base64::output_t output;
    inputStream >> cryptopals::decode_base64(output);
    EXPECT_FALSE(inputStream.fail());
    ASSERT_TRUE(output[0]);
    EXPECT_EQ('M', output[0].get());
    ASSERT_TRUE(output[1]);
    EXPECT_EQ('a', output[1].get());
    ASSERT_TRUE(output[2]);
    EXPECT_EQ('n', output[2].get());
}


TEST(DecodeBase64StreamBuf, ComputesPositionsAndOffsets)
{
    std::istringstream inputStream("VGhpcyBpcyBhIG5pY2UgZGF5Lg==");
    inputStream.ignore(4);
    EXPECT_EQ(4, inputStream.tellg());

    cryptopals::decode_base64_streambuf base64Decoder(inputStream);
    EXPECT_EQ(0,
              base64Decoder.pubseekoff(0,
                                       std::ios_base::cur,
                                       std::ios_base::in));

    std::istream base64DecodedInput(&base64Decoder);
    EXPECT_EQ(0, base64DecodedInput.tellg());
    EXPECT_EQ(4, inputStream.tellg());

    std::string word;
    std::getline(base64DecodedInput, word, 'a');
    EXPECT_EQ("s is ", word);
    EXPECT_EQ(6, base64DecodedInput.tellg());
    EXPECT_EQ(12, inputStream.tellg());

    EXPECT_TRUE(base64DecodedInput.seekg(12));
    EXPECT_TRUE(inputStream);
    EXPECT_EQ(12, base64DecodedInput.tellg());
    EXPECT_EQ(20, inputStream.tellg());

    EXPECT_TRUE(base64DecodedInput.seekg(-3, std::ios_base::cur));
    EXPECT_TRUE(inputStream);
    EXPECT_EQ(9, base64DecodedInput.tellg());
    EXPECT_EQ(16, inputStream.tellg());

#ifdef NDEBUG
    EXPECT_FALSE(base64DecodedInput.seekg(-3, std::ios_base::end));
#else
    EXPECT_DEATH(base64DecodedInput.seekg(-3, std::ios_base::end),
                 "dir == std::ios_base::cur");
#endif
    EXPECT_TRUE(inputStream);
    EXPECT_EQ(16, inputStream.tellg());
}


TEST(SkipChars, SkipsCharacters)
{
    std::istringstream inputStream("This is a nice day.");
    cryptopals::skip_chars_streambuf charSkipper(inputStream, 2, 5);
    std::string result;
    std::getline(std::istream(&charSkipper), result);
    EXPECT_EQ("i cy", result);
}


TEST(BreakRepeatingKeyXor, BreaksRepeatingKeyXorCipher)
{
    std::string fileContents;
    {
        boost::asio::ip::tcp::iostream challenge6File("cryptopals.com", "http");
        challenge6File << "GET /static/challenge-data/6.txt\r\n" << std::flush;
        std::getline(challenge6File, fileContents, '\0');
    }
    std::istrstream fileRead(fileContents.c_str(), fileContents.size());
    cryptopals::decode_base64_streambuf base64Decoder(fileRead);
    std::istream base64DecodedInput(&base64Decoder);
    const std::pair<unsigned int, std::string> & xorKey =
        cryptopals::break_repeating_key_xor_cipher(base64DecodedInput);
    cryptopals::rewind_and_decrypt_using_repeating_xor_key(std::cout,
                                                           base64DecodedInput,
                                                           xorKey.second);
    std::cout << "\nRepeating-key XOR key: " << xorKey.second
              << "\nScore: " << xorKey.first << std::endl;
}
