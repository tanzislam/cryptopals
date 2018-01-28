#ifndef INCLUDED_DETECT_REPEATED_BLOCK
#define INCLUDED_DETECT_REPEATED_BLOCK

#include <utility>
#include <istream>

namespace cryptopals {

///@return Starting offset and number of repetitions of the most repeating block
std::pair<unsigned int, unsigned int> detect_most_repeats(std::istream & input,
                                                          unsigned int size);

}  // namespace cryptopals

#endif
