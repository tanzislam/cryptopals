#include "byte_at_a_time_ecb_decryption_simple.hpp"
#include <sstream>
#include <utility>
#include <stdexcept>
#include <unordered_map>
#include <cassert>
#include <climits>

namespace cryptopals {

unsigned int find_block_size(EcbOracle ecbOracle)
{
    std::string prevCipherText;
    for (auto blockSize = 1u; blockSize < 50u; ++blockSize) {
        std::istringstream plainTextStream(std::string(blockSize, 'A'));
        std::ostringstream cipherTextStream;
        ecbOracle(cipherTextStream, plainTextStream);
        auto cipherText = cipherTextStream.str();
        if (blockSize > 1
            && !cipherText.compare(0,
                                   blockSize - 1,
                                   prevCipherText,
                                   0,
                                   blockSize - 1))
            return blockSize - 1;
        prevCipherText = std::move(cipherText);
    }
    throw std::domain_error("Could not determine block size");
}


static std::unordered_map<std::string, char> make_last_byte_dictionary(
    EcbOracle ecbOracle,
    const std::string & leadingBytes,
    unsigned int blockSize,
    unsigned int blockIdx)
{
    assert(leadingBytes.size() == blockSize * (1 + blockIdx) - 1);
    std::unordered_map<std::string, char> lastBytesDictionary;
    char lastByte = CHAR_MIN;
    do {
        auto input = leadingBytes + lastByte;
        std::ostringstream outputStream;
        ecbOracle(outputStream, std::istringstream(input).seekg(0));
        lastBytesDictionary.emplace(outputStream.str().substr(blockSize
                                                                  * blockIdx,
                                                              blockSize),
                                    lastByte);
    } while (lastByte < CHAR_MAX && (++lastByte, true));
    return lastBytesDictionary;
}


char find_suffix_byte(EcbOracle ecbOracle,
                      const std::string & knownSuffix,
                      unsigned int blockSize)
{
    auto blockIdx = knownSuffix.size() / blockSize;
    std::string leadingBytes(blockSize - knownSuffix.size() % blockSize - 1,
                             'A');
    const auto & lastBytesDict =
        make_last_byte_dictionary(ecbOracle,
                                  leadingBytes + knownSuffix,
                                  blockSize,
                                  blockIdx);
    std::ostringstream outputStream;
    ecbOracle(outputStream, std::istringstream(leadingBytes).seekg(0));
    return lastBytesDict.at(
        outputStream.str().substr(blockSize * blockIdx, blockSize));
}


std::string find_suffix(EcbOracle ecbOracle, unsigned int blockSize)
{
    std::string suffix;
    try {
        while (true) suffix += find_suffix_byte(ecbOracle, suffix, blockSize);
    } catch (std::out_of_range &) {
        // 'suffix' found so far is 1 greater than the true suffix, because on
        // the last 'successful' try the 1st byte of the PKCS#7 padding was
        // included in the dictionary of ciphertexts. The final try was a
        // failure because the actual PKCS#7 padding inside the oracle was
        // different this time, preventing our known suffix from matching
        // anything.
        return suffix.erase(suffix.size() - 1);
    }
}

}  // namespace cryptopals
