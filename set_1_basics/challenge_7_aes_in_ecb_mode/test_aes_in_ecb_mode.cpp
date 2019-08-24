#include <gtest/gtest.h>
#include "download_file_over_https.hpp"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include "decode_base64.hpp"
#include "aes_ecb_decrypt.hpp"
#include <iostream>
#include <sstream>
#include <files.h>
#include <base64.h>


static const std::string & fileData()
{
    static const auto challenge7File =
        cryptopals::downloadFileOverHttps("cryptopals.com",
                                          "/static/challenge-data/7.txt");
    return challenge7File;
}


TEST(AesEcbTest, DecryptsAesInEcbMode)
{
    boost::iostreams::stream<boost::iostreams::array_source>
        challenge7File(fileData().c_str(), fileData().size());
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
        boost::iostreams::stream<boost::iostreams::array_source>
            challenge7File(fileData().c_str(), fileData().size());
        cryptopals::decode_base64_streambuf base64Decoder(challenge7File);
        std::istream base64DecodedInput(&base64Decoder);
        std::ostringstream os;
        base64DecodedInput >> std::noskipws >> os.rdbuf();
        output1 = os.str();
    }
    {
        boost::iostreams::stream<boost::iostreams::array_source>
            challenge7File(fileData().c_str(), fileData().size());
        CryptoPP::FileSource source(challenge7File,
                                    true,
                                    new CryptoPP::Base64Decoder(
                                        new CryptoPP::StringSink(output2)));
    }
    EXPECT_EQ(output1, output2);
}
