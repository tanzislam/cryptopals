#include "aes_ctr.hpp"
#include <aes.h>
#include <boost/endian/buffers.hpp>
#include "aes_ecb_encrypt.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace cryptopals {

aes_ctr_streambuf::aes_ctr_streambuf(std::istream & input,
                                     const std::string & key,
                                     std::uint64_t nonce)
        : d_inputStream(input),
          d_inputBuffer(),
          d_key(key),
          d_nonce(nonce),
          d_counter()
{
    setupNextBlock();
}


void aes_ctr_streambuf::setupNextBlock()
{
    std::string nonceAndCounter;
    nonceAndCounter.reserve(CryptoPP::AES::BLOCKSIZE);
    nonceAndCounter
        .append(boost::endian::little_uint64_buf_t(d_nonce).data(),
                sizeof(boost::endian::little_uint64_buf_t))
        .append(boost::endian::little_uint64_buf_t(d_counter++).data(),
                sizeof(boost::endian::little_uint64_buf_t));

    d_encryptedNonceAndCounter.resize(nonceAndCounter.size());
    aes_ecb_encrypt(boost::iostreams::stream<boost::iostreams::array_sink>(
                        &d_encryptedNonceAndCounter[0],
                        d_encryptedNonceAndCounter.size())
                        .flush(),
                    boost::iostreams::stream<boost::iostreams::array_source>(
                        nonceAndCounter.data(), nonceAndCounter.size())
                        .seekg(0),
                    d_key,
                    false);
}


std::streambuf::int_type aes_ctr_streambuf::underflow()
{
    assert((!gptr() && !egptr()) || (gptr() && egptr() && gptr() == egptr()));

    char inputByte;
    if (!d_inputStream.get(inputByte)) return std::streambuf::underflow();

    if (d_encryptedNonceAndCounter.empty()) setupNextBlock();
    assert(!d_encryptedNonceAndCounter.empty());

    char xorByte = d_encryptedNonceAndCounter.front();
    d_encryptedNonceAndCounter.erase(d_encryptedNonceAndCounter.begin());

    d_inputBuffer = inputByte ^ xorByte;
    setg(&d_inputBuffer, &d_inputBuffer, &d_inputBuffer + 1);
    return std::streambuf::traits_type::to_int_type(d_inputBuffer);
}

}  // namespace cryptopals
