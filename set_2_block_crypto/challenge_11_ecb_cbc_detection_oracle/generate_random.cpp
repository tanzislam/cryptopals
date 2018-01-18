#include "generate_random.hpp"
#include <ctime>
#include <algorithm>

namespace cryptopals {

static std::independent_bits_engine<std::mt19937, CHAR_BIT, unsigned short>
		randomNumbers(time(nullptr));


std::string generateRandomBytes(size_t count)
{
    std::string value(count, '0');
    std::generate(value.begin(), value.end(), randomNumbers);
    return value;
}


std::independent_bits_engine<std::mt19937,
                             CHAR_BIT,
                             unsigned short> & getRandomNumbers()
{
    return randomNumbers;
}

}  // end namespace cryptopals
