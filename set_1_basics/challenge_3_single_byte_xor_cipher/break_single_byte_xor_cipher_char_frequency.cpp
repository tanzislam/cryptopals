#include "break_single_byte_xor_cipher_char_frequency.hpp"
#include <map>
#include <unordered_map>
#include <cctype>
#include <string>
#include "levenshtein_distance.hpp"
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <iomanip>
#include <stdexcept>

namespace cryptopals {

namespace {

/// @todo What if @code unsigned int @endcode is not large enough for the count?
typedef std::map<unsigned char, unsigned int> charFrequencyMap_t;

typedef std::map<uint8_t, charFrequencyMap_t> xorByteToCharFrequencyMap_t;


xorByteToCharFrequencyMap_t getXorByteToCharFrequencyMap(
    std::istream & cipherTextStream)
{
    xorByteToCharFrequencyMap_t xorByteToCharFrequencyMap;
    char cipherTextByte;
    while (cipherTextStream.get(cipherTextByte)) {
        uint8_t xorByte = 0u;
        do {
            ++xorByteToCharFrequencyMap[xorByte][cipherTextByte ^ xorByte];
        } while (++xorByte > 0u);
    }
    return xorByteToCharFrequencyMap;
}


unsigned int characterFrequencyScore(const charFrequencyMap_t & charFrequencies)
{
    std::multimap<unsigned int, char> characterMapByFrequency;
    unsigned int numWhitespace = 0, numPunctuation = 0, numControlChar = 0;
    for (const auto & entry : charFrequencies) {
        unsigned int uppercaseAndLowercaseCombinedFrequency = entry.second;
        char lowercaseEquivalentChar = std::tolower(entry.first);
        if (!std::isprint(entry.first) && !isspace(entry.first))
            ++numControlChar;
        else if (std::isupper(entry.first)) {
            charFrequencyMap_t::const_iterator counterpartEntry =
                charFrequencies.find(lowercaseEquivalentChar);
            if (counterpartEntry != charFrequencies.end())
                uppercaseAndLowercaseCombinedFrequency +=
                    counterpartEntry->second;
        } else if (std::islower(entry.first)) {
            if (charFrequencies.find(std::toupper(entry.first))
                != charFrequencies.end())
                // This lowercase character has already been counted as part of
                // the corresponding uppercase character.
                continue;
        } else if (isspace(entry.first)) {
            numWhitespace += entry.second;
            continue;
        } else if (ispunct(entry.first)) {
            numPunctuation += entry.second;
        } else
            continue;
        characterMapByFrequency.insert(
            std::make_pair(uppercaseAndLowercaseCombinedFrequency,
                           lowercaseEquivalentChar));
    }
    characterMapByFrequency.insert(std::make_pair(numWhitespace, ' '));

    std::string charactersByFrequency;
    charactersByFrequency.reserve(characterMapByFrequency.size());
    boost::range::push_back(charactersByFrequency,
                            characterMapByFrequency
                                | boost::adaptors::map_values
                                | boost::adaptors::reversed);
    return 10000
           - levenshtein_distance(charactersByFrequency.c_str(),
                                  "etaoin shrdlucmfwypvbgkjqxz")
           - numPunctuation * 2 - numControlChar * 50;
}

}  // namespace


std::pair<unsigned int, uint8_t> break_single_byte_xor_cipher_char_frequency::
    do_break(std::istream & cipherTextStream)
{
    const xorByteToCharFrequencyMap_t & xorByteToCharFrequencyMap =
        getXorByteToCharFrequencyMap(cipherTextStream);

    std::multimap<unsigned int, uint8_t> xorByteByScore;
    for (const auto & xorByteEntry : xorByteToCharFrequencyMap)
        xorByteByScore.insert(
            std::make_pair(characterFrequencyScore(xorByteEntry.second),
                           xorByteEntry.first));

    auto winningXorByteItr = xorByteByScore.rbegin();
    if (winningXorByteItr == xorByteByScore.rend() || !winningXorByteItr->first)
        throw std::runtime_error("No winning XOR byte");
    return *winningXorByteItr;
}

}  // namespace cryptopals
