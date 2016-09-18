#include "break_single_byte_xor_cipher_recognize_words.hpp"
#include <limits>
#include <string>
#include <map>
#include "decode_hex.hpp"
#include "hunspell_singleton.hpp"

namespace cryptopals {

std::pair<unsigned int, uint8_t>
    break_single_byte_xor_cipher_recognize_words::do_break(
        std::ostream & plainTextStream,
        std::istream & hexEncodedCipherTextStream
)
{
    static constexpr size_t uint8_range =
            std::numeric_limits<uint8_t>::max() + 1;

    std::string decryptedWord[uint8_range];
    for (auto & str : decryptedWord)
        str.reserve(30);

    unsigned int numRecognizedWords[uint8_range] = { };
    decode_hex::decoded_t cipherTextChar;
    while (hexEncodedCipherTextStream >> decode_hex(cipherTextChar)) {
        uint8_t xorByte = 0u;
        do {
            char decryptedChar = cipherTextChar ^ xorByte;
            if (!isprint(decryptedChar))
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

    const std::pair<unsigned int, uint8_t> & winningXorByte =
            *xorByteByScore.rbegin();
    if (winningXorByte.first == 0u)
        throw std::runtime_error("No winning XOR byte");
    rewindAndDecryptUsingXorByte(plainTextStream,
                                 hexEncodedCipherTextStream,
                                 winningXorByte.second);
    return winningXorByte;
}

}  // close namespace cryptopals
