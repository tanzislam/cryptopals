#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "decode_base64.hpp"
#include "aes_ecb_decrypt.hpp"
#include <iostream>
#include <sstream>
#include <files.h>
#include <base64.h>


TEST(AesEcbTest, DecryptsAesInEcbMode)
{
    boost::asio::ip::tcp::iostream challenge7File("cryptopals.com", "http");
    challenge7File << "GET /static/challenge-data/7.txt\r\n" << std::flush;
    cryptopals::decode_base64_streambuf base64Decoder(challenge7File);
    std::istream base64DecodedInput(&base64Decoder);
    cryptopals::aes_ecb_decrypt(std::cout,
                                base64DecodedInput,
                                "YELLOW SUBMARINE");
}


TEST(DecodeBase64Test, ComparisonWithCryptoPP)
{
    std::string output1, output2;
    {
        boost::asio::ip::tcp::iostream challenge7File("cryptopals.com", "http");
        challenge7File << "GET /static/challenge-data/7.txt\r\n" << std::flush;
        cryptopals::decode_base64_streambuf base64Decoder(challenge7File);
        std::istream base64DecodedInput(&base64Decoder);
        std::ostringstream os;
        base64DecodedInput >> std::noskipws >> os.rdbuf();
        output1 = os.str();
    }
    {
        boost::asio::ip::tcp::iostream challenge7File("cryptopals.com", "http");
        challenge7File << "GET /static/challenge-data/7.txt\r\n" << std::flush;
        CryptoPP::FileSource source(challenge7File,
                                    true,
                                    new CryptoPP::Base64Decoder(
                                        new CryptoPP::StringSink(output2)));
    }
    EXPECT_EQ(output1, output2);
}
