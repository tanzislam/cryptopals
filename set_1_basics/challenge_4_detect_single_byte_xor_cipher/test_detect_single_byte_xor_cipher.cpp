#include <gtest/gtest.h>
#include <sstream>
#include "line_extract_streambuf.hpp"
#include <string>
#include <boost/asio.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "detect_single_byte_xor_cipher.hpp"
#include "break_single_byte_xor_cipher_recognize_words.hpp"
#include "break_single_byte_xor_cipher_char_frequency.hpp"
#include <iostream>


TEST(LineExtractStreamBuf, ExtractsLinesOneAtATimeAndRewindsProperly)
{
    std::stringstream inputStream;
    inputStream << "Hello world!\n"
                   "This is going to extract\r\n"
                   "a line at a time\r"
                   "even for strings with mixed line-endings\n";

    {
        cryptopals::line_extract_streambuf lineExtractor(inputStream);
        std::istream lineExtractorStream(&lineExtractor);
        std::string line;
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("Hello world!", line);

        line.clear();
        EXPECT_EQ("", line);
        lineExtractorStream.clear();
        lineExtractorStream.seekg(0);
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("Hello world!", line);
        EXPECT_TRUE(lineExtractorStream.eof());

        lineExtractorStream.clear();
        EXPECT_EQ(12, lineExtractorStream.tellg());
        line.clear();
        lineExtractorStream >> line;
        EXPECT_TRUE(lineExtractorStream.eof());
        EXPECT_TRUE(line.empty());
    }

    {
        cryptopals::line_extract_streambuf lineExtractor(inputStream);
        std::istream lineExtractorStream(&lineExtractor);
        std::string line;
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("This is going to extract", line);

        line.clear();
        EXPECT_EQ("", line);
        lineExtractorStream.clear();
        lineExtractorStream.seekg(0);
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("This is going to extract", line);
        EXPECT_TRUE(lineExtractorStream.eof());

        lineExtractorStream.clear();
        EXPECT_EQ(24, lineExtractorStream.tellg());
        line.clear();
        lineExtractorStream >> line;
        EXPECT_TRUE(lineExtractorStream.eof());
        EXPECT_TRUE(line.empty());
    }

    {
        cryptopals::line_extract_streambuf lineExtractor(inputStream);
        std::istream lineExtractorStream(&lineExtractor);
        std::string line;
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("a line at a time", line);

        line.clear();
        EXPECT_EQ("", line);
        lineExtractorStream.clear();
        lineExtractorStream.seekg(0);
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("a line at a time", line);
        EXPECT_TRUE(lineExtractorStream.eof());

        lineExtractorStream.clear();
        EXPECT_EQ(16, lineExtractorStream.tellg());
        line.clear();
        lineExtractorStream >> line;
        EXPECT_TRUE(lineExtractorStream.eof());
        EXPECT_TRUE(line.empty());
    }

    {
        cryptopals::line_extract_streambuf lineExtractor(inputStream);
        std::istream lineExtractorStream(&lineExtractor);
        std::string line;
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("even for strings with mixed line-endings", line);

        line.clear();
        EXPECT_EQ("", line);
        lineExtractorStream.clear();
        lineExtractorStream.seekg(0);
        std::getline(lineExtractorStream, line, '\0');
        EXPECT_EQ("even for strings with mixed line-endings", line);
        EXPECT_TRUE(lineExtractorStream.eof());

        lineExtractorStream.clear();
        EXPECT_EQ(40, lineExtractorStream.tellg());
        line.clear();
        lineExtractorStream >> line;
        EXPECT_TRUE(lineExtractorStream.eof());
        EXPECT_TRUE(line.empty());
    }
}


void testDetectSingleByteXorCipher(
    cryptopals::break_single_byte_xor_cipher & breaker)
{
    std::string fileContents;
    {
        boost::asio::ip::tcp::iostream challenge4File("cryptopals.com", "http");
        challenge4File << "GET /static/challenge-data/4.txt\r\n" << std::flush;
        std::getline(challenge4File, fileContents, '\0');
    }
    boost::iostreams::stream<boost::iostreams::array_source>
        fileRead(fileContents.c_str(), fileContents.size());
    auto lineNumberScoreAndXorByte =
        cryptopals::detect_single_byte_xor_cipher(std::cout, fileRead, breaker);
    EXPECT_NE(0, std::get<1>(lineNumberScoreAndXorByte));
    std::cout << "\nMessage was decrypted from line "
              << std::get<0>(lineNumberScoreAndXorByte) + 1 << " using XOR key "
              << std::hex << int(std::get<2>(lineNumberScoreAndXorByte))
              << std::dec << ", with score "
              << std::get<1>(lineNumberScoreAndXorByte) << std::endl;
}


TEST(DetectSingleByteXorCipher, UsingCharacterFrequencyAlgorithm)
{
    testDetectSingleByteXorCipher(
        cryptopals::break_single_byte_xor_cipher_char_frequency().get_ref());
}


TEST(DetectSingleByteXorCipher, UsingWordRecognitionAlgorithm)
{
    testDetectSingleByteXorCipher(
        cryptopals::break_single_byte_xor_cipher_recognize_words().get_ref());
}
