#include <gtest/gtest.h>
#include <string>
#include <boost/asio.hpp>
#include <modes.h>
#include <aes.h>
#include <files.h>
#include <base64.h>
#include <iostream>


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
