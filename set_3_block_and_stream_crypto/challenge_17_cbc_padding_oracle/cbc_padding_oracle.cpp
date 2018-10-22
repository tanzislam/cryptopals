#include "cbc_padding_oracle.hpp"
#include "generate_random.hpp"
#include <sstream>
#include <boost/range/size.hpp>
#include <aes.h>
#include "aes_cbc_encrypt.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <cstring>
#include "aes_cbc_decrypt.hpp"

namespace cryptopals {

static const char * select_random_message()
{
    static const char * const messages[] = {
        "MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
        "MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bX"
        "Bpbic=",
        "MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
        "MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
        "MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
        "MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
        "MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
        "MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
        "MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
        "MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"};

    return messages[getRandomNumbers()() % boost::size(messages)];
}


static const std::string & encryptionKey()
{
    static const auto key = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
    return key;
}


std::pair<std::string, std::string> generate_encrypted_message_and_iv()
{
    const auto initVector = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
    std::ostringstream cipherText;
    auto msg = select_random_message();
    aes_cbc_encrypt(cipherText,
                    boost::iostreams::stream<boost::iostreams::array_source>(
                        msg, std::strlen(msg))
                        .seekg(0),
                    encryptionKey(),
                    initVector.c_str());
    return std::make_pair(cipherText.str(), initVector);
}


bool is_padding_valid(const std::string & ciphertext, const std::string & iv)
{
    std::ostringstream decryptedStream;
    aes_cbc_decrypt(decryptedStream,
                    boost::iostreams::stream<boost::iostreams::array_source>(
                        ciphertext.data(), ciphertext.size())
                        .seekg(0),
                    encryptionKey(),
                    iv.c_str());
    return !!decryptedStream;
}

}  // namespace cryptopals
