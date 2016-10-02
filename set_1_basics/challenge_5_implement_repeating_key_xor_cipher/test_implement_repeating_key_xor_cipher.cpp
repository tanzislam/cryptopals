#include <gtest/gtest.h>
#include <sstream>
#include "encode_hex_streambuf.hpp"
#include "xor_combine.hpp"
#include "repeating_stringstream.hpp"


TEST(ImplementRepeatingKeyXorCipher, EncodeHexStreamBuf)
{
    std::istringstream input("Hello World!!");

    cryptopals::encode_hex_streambuf hexEncoder(input);
    std::istream hexEncodedInput(&hexEncoder);

    std::string hexEncodedInputLine;
    std::getline(hexEncodedInput, hexEncodedInputLine);
    EXPECT_EQ("48656c6c6f20576f726c642121", hexEncodedInputLine);

    hexEncodedInput.seekg(10);
    std::getline(hexEncodedInput, hexEncodedInputLine);
    EXPECT_EQ("20576f726c642121", hexEncodedInputLine);    
}


TEST(ImplementRepeatingKeyXorCipher, EncryptsProperly)
{
    std::istringstream input("Burning 'em, if you ain't quick and nimble\n"
                             "I go crazy when I hear a cymbal");

    cryptopals::encode_hex_streambuf hexEncoderForInput(input);
    std::istream hexEncodedInput(&hexEncoderForInput);

    cryptopals::repeating_stringstream xorKey;
    xorKey << "ICE";
    cryptopals::encode_hex_streambuf hexEncoderForXorKey(xorKey);
    std::istream hexEncodedXorKey(&hexEncoderForXorKey);

    std::ostringstream hexOutputStream;
    cryptopals::xor_combine(hexOutputStream,
                            hexEncodedInput,
                            hexEncodedXorKey);
    EXPECT_EQ("0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a262263"
              "24272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b2028"
              "3165286326302e27282f",
              hexOutputStream.str());
}
