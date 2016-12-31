#include <gtest/gtest.h>
#include <sstream>
#include "pkcs7_unpad.hpp"
#include <string>
#include <boost/asio.hpp>
#include <strstream>
#include "decode_base64.hpp"
#include <aes.h>
#include "aes_cbc_decrypt.hpp"
#include <iostream>
#include <array>
#include "aes_cbc_encrypt.hpp"


TEST(Pkcs7Unpad, UnpadsInputCorrectly)
{
    std::istringstream input("YELLOW SUBMARINE\x04\x04\x04\x04");
    EXPECT_EQ(input.tellg(), 0);

    cryptopals::pkcs7_unpad_streambuf pkcs7Unpadder(input, 20);
    std::istream pkcs7UnpaddedInput(&pkcs7Unpadder);
    EXPECT_EQ(pkcs7UnpaddedInput.tellg(), 0);

    // First extraction
    std::string output;
    std::getline(pkcs7UnpaddedInput, output, '\0');
    EXPECT_EQ(output, "YELLOW SUBMARINE");

    pkcs7UnpaddedInput.clear();
    EXPECT_EQ(pkcs7UnpaddedInput.tellg(), 16);
    input.clear();
    EXPECT_EQ(input.tellg(), 20);

    // Further extraction attempt
    output.clear();
    std::getline(pkcs7UnpaddedInput, output, '\0');
    EXPECT_TRUE(output.empty());

    // Rewind
    pkcs7UnpaddedInput.clear();
    pkcs7UnpaddedInput.seekg(0);
    EXPECT_EQ(pkcs7UnpaddedInput.tellg(), 0);
    EXPECT_EQ(input.tellg(), 0);

    // Second extraction
    std::getline(pkcs7UnpaddedInput, output, '\0');
    EXPECT_EQ(output, "YELLOW SUBMARINE");

    pkcs7UnpaddedInput.clear();
    EXPECT_EQ(pkcs7UnpaddedInput.tellg(), 16);
    input.clear();
    EXPECT_EQ(input.tellg(), 20);
}


TEST(Pkcs7Unpad, UnpadsOutputCorrectly)
{
    std::istringstream input("YELLOW SUBMARINE\x04\x04\x04\x04");
    EXPECT_EQ(input.tellg(), 0);

    std::ostringstream output;
    cryptopals::pkcs7_unpad_streambuf pkcs7Unpadder(output, 20);

    // First extraction
    input.get(pkcs7Unpadder, '\0');
    pkcs7Unpadder.pubsync();
    EXPECT_EQ(output.str(), "YELLOW SUBMARINE");

    input.clear();
    EXPECT_EQ(input.tellg(), 20);

    // Further extraction attempt
    output.str("");
    output.clear();
    input.get(pkcs7Unpadder, '\0');
    EXPECT_TRUE(output.str().empty());
}


TEST(AesCbcMode, CanDecrypt)
{
    std::string fileContents;
    {
        boost::asio::ip::tcp::iostream challenge10File("cryptopals.com",
                                                       "http");
        challenge10File << "GET /static/challenge-data/10.txt\r\n"
                        << std::flush;
        std::getline(challenge10File, fileContents, '\0');
    }
    std::istrstream challenge10File(fileContents.c_str(), fileContents.size());
    cryptopals::decode_base64_streambuf base64Decoder(challenge10File);
    cryptopals::aes_cbc_decrypt(std::cout,
                                std::istream(&base64Decoder).seekg(0),
                                "YELLOW SUBMARINE",
                                std::array<char,
                                           CryptoPP::AES::BLOCKSIZE>{0}.data());
    EXPECT_TRUE(challenge10File.eof());
}


TEST(AesCbcMode, CanEncrypt)
{
    std::stringstream inputStream("This is a nice day"), encryptedStream;
    char key[] = "YELLOW SUBMARINE";
    cryptopals::aes_cbc_encrypt(encryptedStream,
                                inputStream,
                                key,
                                std::array<char,
                                           CryptoPP::AES::BLOCKSIZE>{0}.data());
    std::ostringstream decryptionResult;
    cryptopals::aes_cbc_decrypt(decryptionResult,
                                encryptedStream,
                                key,
                                std::array<char,
                                           CryptoPP::AES::BLOCKSIZE>{0}.data());
    EXPECT_EQ(inputStream.str(), decryptionResult.str());
}
