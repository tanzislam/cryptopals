#include <gtest/gtest.h>
#include "hamming_distance.hpp"


TEST(BreakRepeatingKeyXor, HammingDistance)
{
    EXPECT_EQ(37,
              cryptopals::hamming_distance("this is a test", "wokka wokka!!!"));
}
