#ifndef INCLUDED_GENERATE_RANDOM
#define INCLUDED_GENERATE_RANDOM

#include <string>
#include <random>
#include <climits>

namespace cryptopals {

std::string generateRandomBytes(size_t count);

std::independent_bits_engine<std::mt19937, CHAR_BIT, unsigned short> &
    getRandomNumbers();

}  // namespace cryptopals

#endif
