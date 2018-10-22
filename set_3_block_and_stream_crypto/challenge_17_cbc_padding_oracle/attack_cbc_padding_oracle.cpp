#include "attack_cbc_padding_oracle.hpp"
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include "pkcs7_unpad_wrap.hpp"

namespace cryptopals {

static char find_plaintext_byte(bool (*oracle)(const std::string &,
                                               const std::string &),
                                const std::string & ciphertext,
                                const std::string & initVector,
                                const std::string & knownEndPlaintextBytes)
{
    const auto blockSize = initVector.size();
    const auto posToFindFromEnd = knownEndPlaintextBytes.size() + 1u;

    auto initVectorAndCiphertext = initVector + ciphertext;
    std::transform(knownEndPlaintextBytes.rbegin(),
                   knownEndPlaintextBytes.rend(),
                   initVectorAndCiphertext.rbegin() + blockSize,
                   initVectorAndCiphertext.rbegin() + blockSize,
                   [posToFindFromEnd](unsigned char knownByte,
                                      unsigned char cipherByte) {
                       return cipherByte ^ (knownByte ^ posToFindFromEnd);
                   });
    for (auto attempt = 0x00u; attempt < 0x100u; ++attempt) {
        auto thisInitVectorAndCiphertext = initVectorAndCiphertext;
        *(thisInitVectorAndCiphertext.rbegin() + blockSize
          + knownEndPlaintextBytes.size()) ^= attempt;
        if (!oracle(thisInitVectorAndCiphertext.substr(blockSize),
                    thisInitVectorAndCiphertext.substr(0, blockSize)))
            continue;

        // Check if input was already padded correctly
        if (attempt == 0x00u) {
            *(thisInitVectorAndCiphertext.rbegin() + blockSize
              + knownEndPlaintextBytes.size() + 1) ^= 0xFFu;
            if (!oracle(thisInitVectorAndCiphertext.substr(blockSize),
                        thisInitVectorAndCiphertext.substr(0, blockSize)))
                continue;
        }

        return attempt ^ posToFindFromEnd;
    }
    throw std::runtime_error("Could not find valid plaintext byte");
}


static std::string find_last_plaintext_block(
    bool (*oracle)(const std::string &, const std::string &),
    const std::string & ciphertext,
    const std::string & initVector)
{
    const auto blockSize = initVector.size();
    std::string endPlaintextBytes;
    for (auto i = 0u; i < blockSize; ++i)
        endPlaintextBytes.insert(0,
                                 1,
                                 find_plaintext_byte(oracle,
                                                     ciphertext,
                                                     initVector,
                                                     endPlaintextBytes));
    return endPlaintextBytes;
}


std::string attack_cbc_padding_oracle(bool (*oracle)(const std::string &,
                                                     const std::string &),
                                      const std::string & ciphertext,
                                      const std::string & initVector)
{
    const auto blockSize = initVector.size();
    assert(ciphertext.size() >= blockSize);
    assert(ciphertext.size() % blockSize == 0);
    assert(oracle(ciphertext, initVector));

    std::string plaintext;
    for (auto shortCiphertext = ciphertext; !shortCiphertext.empty();
         shortCiphertext.erase(shortCiphertext.size() - blockSize))
        plaintext.insert(0,
                         find_last_plaintext_block(oracle,
                                                   shortCiphertext,
                                                   initVector));
    return unpad_pkcs7(plaintext);
}

}  // namespace cryptopals
