#include "detect_repeated_block.hpp"
#include <boost/scope_exit.hpp>
#include <vector>
#include <string>
#include <algorithm>

namespace cryptopals {

std::pair<unsigned int, unsigned int> detect_most_repeats(std::istream & input,
                                                          unsigned int size)
{
    std::pair<unsigned int, unsigned int> maxRepeat;
    char * thisBlock = new char[size];
    BOOST_SCOPE_EXIT_ALL(=) { delete[] thisBlock; };
    std::vector<std::string> blocksReadSoFar;
    while (input.read(thisBlock, size)) {
        std::string thisBlockStr(thisBlock, size);
        auto numOccurrences = std::count(blocksReadSoFar.begin(),
                                         blocksReadSoFar.end(),
                                         thisBlockStr);
        if (numOccurrences > maxRepeat.second)
            maxRepeat = std::make_pair(std::streamoff(input.tellg()) - size + 1,
                                       numOccurrences + 1);
        blocksReadSoFar.push_back(thisBlockStr);
    }
    return maxRepeat;
}

}  // namespace cryptopals
