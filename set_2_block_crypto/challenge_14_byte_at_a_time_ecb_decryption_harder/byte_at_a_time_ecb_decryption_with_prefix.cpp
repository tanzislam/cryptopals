#include "byte_at_a_time_ecb_decryption_with_prefix.hpp"
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <cassert>
#include <climits>

namespace cryptopals {

std::pair<unsigned int, unsigned int> find_block_size_and_prefix_length(
    EcbOracle ecbOracle,
    const unsigned int minBlockSize,
    const unsigned int tolerance,
    const unsigned int stability)
{
    auto candidateBlockSize = 0u, candidateBlockSizeMinusPrefix = 0u;
    std::string prevCipherText;
    for (auto msgLength = 1u; msgLength < 150u; ++msgLength) {
        std::istringstream plainTextStream(std::string(msgLength, 'A'));
        std::ostringstream cipherTextStream;
        ecbOracle(cipherTextStream, plainTextStream);
        auto cipherText = cipherTextStream.str();
        auto trialBlockSize =
            std::distance(prevCipherText.begin(),
                          std::mismatch(prevCipherText.begin(),
                                        prevCipherText.end(),
                                        cipherText.begin())
                              .first);
        if (trialBlockSize >= minBlockSize) {
            if (!candidateBlockSize) {
                candidateBlockSize = trialBlockSize;
                candidateBlockSizeMinusPrefix = msgLength - 1;
            } else if (candidateBlockSize - trialBlockSize > tolerance) {
                candidateBlockSize = 0u;
                candidateBlockSizeMinusPrefix = 0u;
            } else if (msgLength - candidateBlockSizeMinusPrefix <= stability)
                candidateBlockSize = trialBlockSize;
            else
                return std::make_pair(candidateBlockSize,
                                      candidateBlockSize
                                          - candidateBlockSizeMinusPrefix);
        } else {
            candidateBlockSize = 0u;
            candidateBlockSizeMinusPrefix = 0u;
        }
        prevCipherText = std::move(cipherText);
    }
    throw std::domain_error("Could not determine block size and prefix length");
}


static std::unordered_map<std::string, char> make_last_byte_dictionary(
    EcbOracle ecbOracle,
    const std::string & leadingBytes,
    unsigned int blockSize,
    unsigned int blockIdx,
    unsigned int prefixLength)
{
    assert(prefixLength + leadingBytes.size()
           == blockSize * (1 + blockIdx) - 1);
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


char find_suffix_byte_with_prefix(EcbOracle ecbOracle,
                                  const std::string & knownSuffix,
                                  unsigned int blockSize,
                                  unsigned int prefixLength)
{
    auto blockIdx = (prefixLength + knownSuffix.size()) / blockSize;
    std::string leadingBytes(blockSize
                                 - (prefixLength + knownSuffix.size())
                                       % blockSize
                                 - 1,
                             'A');
    const auto & lastBytesDict =
        make_last_byte_dictionary(ecbOracle,
                                  leadingBytes + knownSuffix,
                                  blockSize,
                                  blockIdx,
                                  prefixLength);
    std::ostringstream outputStream;
    std::istringstream inputStream(leadingBytes);
    ecbOracle(outputStream, inputStream);
    return lastBytesDict.at(
        outputStream.str().substr(blockSize * blockIdx, blockSize));
}


std::string find_suffix_with_prefix(EcbOracle ecbOracle,
                                    unsigned int blockSize,
                                    unsigned int prefixLength)
{
    std::string suffix;
    try {
        while (true)
            suffix += find_suffix_byte_with_prefix(ecbOracle,
                                                   suffix,
                                                   blockSize,
                                                   prefixLength);
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
