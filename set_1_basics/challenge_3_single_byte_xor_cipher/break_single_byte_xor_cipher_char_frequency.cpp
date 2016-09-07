#include "break_single_byte_xor_cipher_char_frequency.hpp"
#include <map>
#include <unordered_map>
#include "decode_hex.hpp"
#include <cctype>
#include <string>
#include <algorithm>
#include <iterator>
#include "repeating_stringstream.hpp"
#include "encode_hex.hpp"
#include <iomanip>
#include <stdexcept>
#include "xor_combine.hpp"

namespace cryptopals {

namespace {

/// @todo What if @code unsigned int @endcode is not large enough for the count?
typedef std::unordered_map<char, unsigned int> charFrequencyMap_t;

typedef std::map<uint8_t, charFrequencyMap_t> xorByteToCharFrequencyMap_t;


xorByteToCharFrequencyMap_t getXorByteToCharFrequencyMap(
        std::istream & hexEncodedCipherTextStream
)
{
    xorByteToCharFrequencyMap_t xorByteToCharFrequencyMap;
    decode_hex::decoded_t cipherTextByte;
    while (hexEncodedCipherTextStream >> decode_hex(cipherTextByte)) {
        uint8_t xorByte = 0u;
        do {
            ++xorByteToCharFrequencyMap[xorByte][cipherTextByte ^ xorByte];
        } while (++xorByte > 0u);
    }
    return xorByteToCharFrequencyMap;
}


unsigned int characterFrequencyScore(const charFrequencyMap_t & charFrequencies)
{
    std::multimap<unsigned int, char> characterByFrequency;
    unsigned int whitespaceFrequency = 0;
    for (const auto & entry : charFrequencies) {
        unsigned int uppercaseAndLowercaseCombinedFrequency = entry.second;
        char lowercaseEquivalentChar = std::tolower(entry.first);
        if (!std::isprint(entry.first)) return 0;
        if (std::isupper(entry.first)) {
            charFrequencyMap_t::const_iterator counterpartEntry =
                    charFrequencies.find(lowercaseEquivalentChar);
            if (counterpartEntry != charFrequencies.end())
                uppercaseAndLowercaseCombinedFrequency +=
                        counterpartEntry->second;
        } else if (std::islower(entry.first)) {
            if (charFrequencies.find(std::toupper(entry.first)) !=
                    charFrequencies.end())
                // This lowercase character has already been counted as part of
                // the corresponding uppercase character.
                continue;
        } else if (isspace(entry.first)) {
            whitespaceFrequency += entry.second;
            continue;
        }
        else continue;
        characterByFrequency.insert(
                std::make_pair(uppercaseAndLowercaseCombinedFrequency,
                               lowercaseEquivalentChar)
        );
    }
    characterByFrequency.insert(std::make_pair(whitespaceFrequency, ' '));

    const char * englishLettersByFrequency = "etaoin shrdlucmfwypvbgkjqxz";
    const auto & mismatchedEntries =
            std::mismatch(characterByFrequency.rbegin(),
                          characterByFrequency.rend(),
                          englishLettersByFrequency,
                          [](const std::pair<unsigned int, char> & a, char b)
                                -> bool { return a.second < b; });
    return std::distance(englishLettersByFrequency, mismatchedEntries.second);
}


void rewindAndDecryptUsingXorByte(std::ostream & outputStream,
                                  std::istream & hexEncodedCipherTextStream,
                                  uint8_t xorByte)
{
    hexEncodedCipherTextStream.clear();
    if (!hexEncodedCipherTextStream.seekg(0))
        throw std::runtime_error("Could not reset hexEncodedCipherTextStream");

    xor_combine(outputStream,
                hexEncodedCipherTextStream,
                reinterpret_cast<std::istream &>(
                        repeating_stringstream() << encode_hex(xorByte)
                ),
                false);
}

}  // close unnamed namespace


std::pair<unsigned int, uint8_t> break_single_byte_xor_cipher_char_frequency(
        std::ostream & plainTextStream,
        std::istream & hexEncodedCipherTextStream
)
{
    const xorByteToCharFrequencyMap_t & xorByteToCharFrequencyMap =
            getXorByteToCharFrequencyMap(hexEncodedCipherTextStream);

    std::multimap<unsigned int, uint8_t> xorByteByScore;
    for (const auto & xorByteEntry : xorByteToCharFrequencyMap)
        xorByteByScore.insert(
                std::make_pair(characterFrequencyScore(xorByteEntry.second),
                               xorByteEntry.first)
        );

    auto winningXorByteItr = xorByteByScore.rbegin();
    if (winningXorByteItr == xorByteByScore.rend())
        throw std::runtime_error("No winning XOR byte");
    rewindAndDecryptUsingXorByte(plainTextStream,
                                 hexEncodedCipherTextStream,
                                 winningXorByteItr->second);
    return *winningXorByteItr;
}

}  // close namespace cryptopals
