#include <gtest/gtest.h>
#include <sstream>
#include "decode_base64.hpp"
#include "aes_ctr.hpp"
#include <iostream>
#include <string>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "generate_random.hpp"
#include <aes.h>


TEST(AesCtr, DecryptsGivenCiphertext)
{
    std::istringstream input("L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyM"
                             "TJULu/6/kXX0KSvoOLSFQ==");

    cryptopals::decode_base64_streambuf base64Decoder(input);
    std::istream base64DecodedInput(&base64Decoder);

    cryptopals::aes_ctr_streambuf aesCtrCodec(base64DecodedInput,
                                              "YELLOW SUBMARINE",
                                              0);
    std::cout << &aesCtrCodec << '\n';
}


TEST(AesCtr, EncryptsAndDecryptsOtherTexts)
{
    std::string texts[] =
        {"The quick brown fox jumps over the lazy dog",
         "Jackdaws love my big sphinx of quartz",
         "lorem ipsum quia dolor sit amet, consectetur, adipisci velit"};
    for (const auto & text : texts) {
        boost::iostreams::stream<boost::iostreams::array_source>
            input(text.c_str(), text.size());

        auto key = cryptopals::generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
        std::uint64_t nonce = cryptopals::getRandomNumbers()();
        cryptopals::aes_ctr_streambuf aesCtrEncrypt(input, key, nonce);

        std::stringstream encrypted;
        encrypted << &aesCtrEncrypt;

        cryptopals::aes_ctr_streambuf aesCtrDecrypt(encrypted, key, nonce);

        std::string decrypted;
        std::getline(std::istream(&aesCtrDecrypt), decrypted);
        EXPECT_EQ(text, decrypted);
    }
}
