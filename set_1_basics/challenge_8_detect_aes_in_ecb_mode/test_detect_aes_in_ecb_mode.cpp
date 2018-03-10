#include <gtest/gtest.h>
#include <sstream>
#include "detect_repeated_block.hpp"
#include <boost/asio.hpp>
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
    std::string fileContents;
    {
        boost::asio::ip::tcp::iostream challenge8File("cryptopals.com", "http");
        challenge8File << "GET /static/challenge-data/8.txt\r\n" << std::flush;
        std::getline(challenge8File, fileContents, '\0');
    }
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
