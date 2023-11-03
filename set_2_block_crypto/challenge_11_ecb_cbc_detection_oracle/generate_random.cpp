#include "generate_random.hpp"
#include <ctime>
#include <algorithm>
#include <iostream>

namespace cryptopals {

std::string generateRandomBytes(size_t count)
{
    std::string value(count, '0');
    std::generate(value.begin(), value.end(), getRandomNumbers());
    return value;
}


std::independent_bits_engine<std::mt19937, CHAR_BIT, unsigned short> &
    getRandomNumbers()
{
    static auto seed = time(nullptr);
    static std::independent_bits_engine<std::mt19937, CHAR_BIT, unsigned short>
        randomNumberGenerator(
            (std::clog << "[DEBUG] seed: " << seed << std::endl, seed));
    return randomNumberGenerator;
}

}  // namespace cryptopals
