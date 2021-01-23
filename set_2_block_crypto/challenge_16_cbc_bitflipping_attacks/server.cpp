#include "server.hpp"
#include "generate_random.hpp"
#include <aes.h>
#include <boost/algorithm/string/erase.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "aes_cbc_encrypt.hpp"
#include <sstream>
#include "aes_cbc_decrypt.hpp"
#include "key_value_parser.hpp"

namespace cryptopals {

namespace {

static const std::string & key()
{
    static auto key = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
    return key;
}

static const std::string & initVector()
{
    static auto initVector = generateRandomBytes(CryptoPP::AES::BLOCKSIZE);
    return initVector;
}

}  // namespace


std::string generate_encrypted_cookie(const std::string & data)
{
    auto strippedData = data;
    boost::algorithm::erase_all(strippedData, ";");
    boost::algorithm::erase_all(strippedData, "=");
    const auto stringToEncrypt = "comment1=cooking%20MCs;userdata="
                                 + strippedData
                                 + ";comment2=%20like%20a%20pound%20of%20bacon";
    boost::iostreams::stream<boost::iostreams::array_source>
        inputStream(stringToEncrypt.c_str(), stringToEncrypt.size());
    std::ostringstream output;
    aes_cbc_encrypt(output, inputStream, key(), initVector().data());
    return output.str();
}


bool is_admin_cookie(const std::string & encryptedCookie)
{
    boost::iostreams::stream<boost::iostreams::array_source>
        inputStream(encryptedCookie.c_str(), encryptedCookie.size());
    std::ostringstream decryptedCookie;
    aes_cbc_decrypt(decryptedCookie, inputStream, key(), initVector().data());
    return parse_structured_cookie(decryptedCookie.str(), ';')["admin"]
           == "true";
}

}  // namespace cryptopals
