#include <gtest/gtest.h>
#include <sstream>
#include "levenshtein_distance.hpp"
#include <string>
#include "break_single_byte_xor_cipher.hpp"
#include "decode_hex.hpp"
#include <iostream>
#include "break_single_byte_xor_cipher_char_frequency.hpp"
#include "break_single_byte_xor_cipher_recognize_words.hpp"


TEST(LevenshteinDistance, ComputesDistanceCorrectly)
{
    using namespace cryptopals;
    EXPECT_EQ(3, levenshtein_distance("kitten", "sitting"));
    EXPECT_EQ(3, levenshtein_distance("saturday", "sunday"));
    EXPECT_EQ(2, levenshtein_distance("gumbo", "gambol"));
    EXPECT_EQ(4, levenshtein_distance("meilenstein", "levenshtein"));
}


void testBreakSingleByteXorCipher(
        cryptopals::break_single_byte_xor_cipher & breaker
)
{
    std::istringstream hexEncodedCipherText(
        "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
    );
    std::ostringstream plainTextOutput;
    cryptopals::decode_hex_streambuf hexDecoder(hexEncodedCipherText);
    std::pair<unsigned int, uint8_t> scoreAndXorKeyByte =
            breaker.do_break(plainTextOutput,
                             std::istream(&hexDecoder).seekg(0));
    EXPECT_NE(0, scoreAndXorKeyByte.first);
    EXPECT_NE(0, scoreAndXorKeyByte.second);
    EXPECT_NE(0, plainTextOutput.str().size());
    std::cout << "XOR key byte was: " << std::hex << scoreAndXorKeyByte.second
              << std::dec << "\nMessage was: " << plainTextOutput.str()
              << "\nScore was: " << scoreAndXorKeyByte.first
              << std::endl;
}


TEST(BreakSingleByteXorCipher, UsingCharacterFrequencyAlgorithm)
{
    testBreakSingleByteXorCipher(
            cryptopals::break_single_byte_xor_cipher_char_frequency().get_ref()
    );
}


TEST(BreakSingleByteXorCipher, UsingWordRecognitionAlgorithm)
{
    testBreakSingleByteXorCipher(
            cryptopals::break_single_byte_xor_cipher_recognize_words().get_ref()
    );
}
