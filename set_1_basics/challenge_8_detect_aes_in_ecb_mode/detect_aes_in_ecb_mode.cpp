#include "detect_aes_in_ecb_mode.hpp"
#include "line_extract_streambuf.hpp"
#include "decode_hex.hpp"
#include "detect_repeated_block.hpp"

namespace cryptopals {

std::tuple<unsigned int, unsigned int, unsigned int> detect_aes_in_ecb_mode(
    std::istream & input)
{
    auto lineNumberOffsetAndRepeatCount = std::make_tuple(0u, 0u, 0u);
    for (unsigned int lineNumber = 1; input; ++lineNumber) {
        line_extract_streambuf thisLine(input);
        std::istream thisLineStream(&thisLine);
        decode_hex_streambuf hexDecoder(thisLineStream);
        auto mostRepeats =
            detect_most_repeats(std::istream(&hexDecoder).seekg(0), 16);
        if (mostRepeats.second > std::get<2>(lineNumberOffsetAndRepeatCount))
            lineNumberOffsetAndRepeatCount =
                std::make_tuple(lineNumber,
                                (mostRepeats.first - 1 * 2) + 1,
                                mostRepeats.second);
    }
    return lineNumberOffsetAndRepeatCount;
}

}  // namespace cryptopals
