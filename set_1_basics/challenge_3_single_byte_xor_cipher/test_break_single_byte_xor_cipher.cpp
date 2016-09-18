#include <gtest/gtest.h>
#include "repeating_stringstream.hpp"
#include <sstream>
#include "break_single_byte_xor_cipher.hpp"
#include <iostream>
#include "break_single_byte_xor_cipher_char_frequency.hpp"
#include "break_single_byte_xor_cipher_recognize_words.hpp"


TEST(RepeatingStringStream, RepeatsStringAsStream)
{
    cryptopals::repeating_stringstream sstream;
    sstream << "helloworld";

    std::string extractedChars;
    sstream >> std::setw(7) >> extractedChars;
    EXPECT_EQ("hellowo", extractedChars);

    sstream >> std::setw(7) >> extractedChars;
    EXPECT_EQ("rldhell", extractedChars);
    EXPECT_TRUE(sstream);

    sstream >> std::setw(5) >> extractedChars;
    EXPECT_EQ("oworl", extractedChars);

    sstream << "!ExtensionTest";
    sstream >> std::setw(12) >> extractedChars;
    EXPECT_EQ("d!ExtensionT", extractedChars);

    sstream >> std::setw(10) >> extractedChars;
    EXPECT_EQ("esthellowo", extractedChars);
}


void testBreakSingleByteXorCipher(
        cryptopals::break_single_byte_xor_cipher & breaker
)
{
    std::istringstream hexEncodedCipherText(
        "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736"
    );
    std::ostringstream plainTextOutput;
    std::pair<unsigned int, uint8_t> scoreAndXorKeyByte =
            breaker.do_break(plainTextOutput, hexEncodedCipherText);
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
