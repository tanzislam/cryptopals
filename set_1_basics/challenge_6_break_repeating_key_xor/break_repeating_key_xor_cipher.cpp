#include "break_repeating_key_xor_cipher.hpp"
#include <map>
#include "hamming_distance.hpp"
#include "skip_chars.hpp"
#include "break_single_byte_xor_cipher_char_frequency.hpp"
#include "repeating_stringstream.hpp"
#include "xor.hpp"

namespace cryptopals {

namespace {

std::multimap<double, unsigned int> sortKeySizeByScore(std::istream & input)
{
    std::multimap<double, unsigned int> keySizeByScoreMap;
    for (
            std::string portion1(1, input.get()), portion2(1, input.get()),
                        portion3(1, input.get()), portion4(1, input.get());
            input && portion1.size() <= 40;
            (((portion4 += input.get()) += input.get()) += input.get())
                    += input.get(),
                portion3 += portion4.substr(0, 3), portion4.erase(0, 3),
                portion2 += portion3.substr(0, 2), portion3.erase(0, 2),
                portion1 += portion2.substr(0, 1), portion2.erase(0, 1)
    ) {
        double score1 =
                1000.00 - hamming_distance(portion1.c_str(), portion2.c_str())
                                / static_cast<double>(portion1.size());
        double score2 =
                1000.00 - hamming_distance(portion3.c_str(), portion4.c_str())
                                / static_cast<double>(portion3.size());
        keySizeByScoreMap.emplace((score1 + score2) / 2.0, portion1.size());
    }
    return keySizeByScoreMap;
}

}  // close unnamed namespace


std::pair<unsigned int, std::string> break_repeating_key_xor_cipher(
        std::istream & inputStream
)
{
    const std::multimap<double, unsigned int> & keySizeByScoreMap =
            sortKeySizeByScore(inputStream);
    std::pair<unsigned int, std::string> winningXorKey;
    for (
            auto keySizeByScoreMapIt = keySizeByScoreMap.rbegin();
            keySizeByScoreMapIt != keySizeByScoreMap.rend();
            ++keySizeByScoreMapIt
    ) {
        unsigned int keySize = keySizeByScoreMapIt->second;
        try {
            std::string xorKey;
            unsigned int xorKeyScore = 0;
            for (unsigned int offset = 0; offset < keySize; ++offset) {
                inputStream.clear();
                inputStream.seekg(0);
                if (!inputStream) break;
                skip_chars_streambuf charSkip(inputStream, offset, keySize);
                std::istream everyNthCharacter(&charSkip);
                auto xorKeyWithScore =
                    break_single_byte_xor_cipher_char_frequency().do_break(
                            everyNthCharacter
                    );
                xorKeyScore += xorKeyWithScore.first;
                xorKey += xorKeyWithScore.second;
            }
            xorKeyScore /= keySize;
            if (xorKeyScore > winningXorKey.first) {
                winningXorKey.first = xorKeyScore;
                winningXorKey.second = xorKey;
                if (xorKeyScore >= 9950) break;
            }
        } catch (std::runtime_error & e) {
            continue;
        }
    }

    if (winningXorKey.second.empty())
        throw std::runtime_error("No winning XOR byte");
    return winningXorKey;
}


void rewind_and_decrypt_using_repeating_xor_key(std::ostream & outputStream,
                                                std::istream & cipherTextStream,
                                                const std::string & xorKey)
{
    cipherTextStream.clear();
    cipherTextStream.seekg(0);
    repeating_stringstream repeatingXorKey;
    repeatingXorKey << xorKey;
    xor_streambuf xorCombiner(cipherTextStream, repeatingXorKey);
    std::istream(&xorCombiner).get(*outputStream.rdbuf(), '\0');
}

}  // close namespace cryptopals
