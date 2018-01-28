#include "hamming_distance.hpp"
#include <cassert>
#include <bitset>

namespace cryptopals {

unsigned int hamming_distance(const char * input1, const char * input2)
{
    assert(input1 && input2);
    unsigned int numBitsDifferent = 0;
    while (*input1 && *input2) {
        numBitsDifferent += std::bitset<8>(*input1++ ^ *input2++).count();
    }
    return numBitsDifferent;
}

}  // namespace cryptopals
