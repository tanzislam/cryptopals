#include <gtest/gtest.h>
#include <sstream>
#include "detect_repeated_block.hpp"
#include "download_file_over_https.hpp"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "detect_aes_in_ecb_mode.hpp"
#include <iostream>


TEST(DetectRepeatedBlock, CountsRepeatedBlock)
{
    std::stringstream input;
    input << "Thisisaniceday..ThisniceThis....This";
    auto mostRepeats = cryptopals::detect_most_repeats(input, 4);
    EXPECT_EQ(33, mostRepeats.first);
    EXPECT_EQ(4, mostRepeats.second);
}


TEST(DetectAesInEcbMode, DetectsAesEcbByRepeatCount)
{
    auto fileContents =
        cryptopals::downloadFileOverHttps("cryptopals.com",
                                          "/static/challenge-data/8.txt");
    boost::iostreams::stream<boost::iostreams::array_source>
        challenge8File(fileContents.c_str(), fileContents.size());

    auto lineNumberOffsetAndRepeatCount =
        cryptopals::detect_aes_in_ecb_mode(challenge8File);
    std::cout << "Line number with most repeated 16-byte block: #"
              << std::get<0>(lineNumberOffsetAndRepeatCount)
              << "\nThe 16-byte block occurred "
              << std::get<2>(lineNumberOffsetAndRepeatCount)
              << " times and started at character position "
              << std::get<1>(lineNumberOffsetAndRepeatCount) << std::endl;

    EXPECT_GT(std::get<0>(lineNumberOffsetAndRepeatCount), 0);
    EXPECT_GT(std::get<1>(lineNumberOffsetAndRepeatCount), 0);
    EXPECT_GT(std::get<2>(lineNumberOffsetAndRepeatCount), 0);
}
