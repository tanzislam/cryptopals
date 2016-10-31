#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <string>
#include <modes.h>
#include <aes.h>
#include <files.h>
#include <base64.h>
#include <iostream>
#include "decode_base64.hpp"
#include <sstream>


TEST(AesEcbTest, DecryptsAesInEcbMode)
{
    boost::asio::ip::tcp::iostream challenge7File("cryptopals.com", "http");
    challenge7File << "GET /static/challenge-data/7.txt\r\n" << std::flush;
    std::string key = "YELLOW SUBMARINE";
    CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption aesDecryptor(
            reinterpret_cast<const unsigned char *>(key.c_str()),
            key.size()
    );
    CryptoPP::FileSource(
            challenge7File,
            true,
            new CryptoPP::Base64Decoder(
                    new CryptoPP::StreamTransformationFilter(
                            aesDecryptor,
                            new CryptoPP::FileSink(std::cout)
                    )
            )
    );
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
        CryptoPP::FileSource(
                challenge7File,
                true,
                new CryptoPP::Base64Decoder(
                       new CryptoPP::StringSink(output2)
                )
        );
    }
    EXPECT_EQ(output1, output2);
}
