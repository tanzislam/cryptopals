#include "break_single_byte_xor_cipher_recognize_words.hpp"
#include <limits>
#include <string>
#include <map>
#include "hunspell_singleton.hpp"

namespace cryptopals {

std::pair<unsigned int, uint8_t>
    break_single_byte_xor_cipher_recognize_words::do_break(
        std::istream & cipherTextStream)
{
    static constexpr size_t uint8_range = std::numeric_limits<uint8_t>::max()
                                          + 1;

    std::string decryptedWord[uint8_range];
    for (auto & str : decryptedWord) str.reserve(30);

    unsigned int numRecognizedWords[uint8_range] = {};
    char cipherTextChar;
    while (cipherTextStream.get(cipherTextChar)) {
        uint8_t xorByte = 0u;
        do {
            unsigned char decryptedChar = cipherTextChar ^ xorByte;
            if (!isprint(decryptedChar) && !isspace(decryptedChar))
                numRecognizedWords[xorByte] = 0u;
            else if (isspace(decryptedChar)) {
                if (HunspellSingleton::inst().spell(decryptedWord[xorByte]))
                    ++numRecognizedWords[xorByte];
                decryptedWord[xorByte].clear();
            } else
                decryptedWord[xorByte].push_back(decryptedChar);
        } while (++xorByte > 0u);
    }

    std::map<unsigned int, uint8_t> xorByteByScore;
    uint8_t xorByte = 0u;
    do {
        xorByteByScore.emplace(numRecognizedWords[xorByte], xorByte);
    } while (++xorByte > 0u);

    const auto & winningXorByteItr = xorByteByScore.rbegin();
    if (winningXorByteItr == xorByteByScore.rend() || !winningXorByteItr->first)
        throw std::runtime_error("No winning XOR byte");
    return *winningXorByteItr;
}

}  // namespace cryptopals
