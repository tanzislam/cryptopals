#include "forge_admin_profile_cbc.hpp"
#include "byte_at_a_time_ecb_decryption_with_prefix.hpp"
#include <tuple>
#include "server.hpp"

namespace cryptopals {

std::pair<unsigned int, unsigned int> find_block_size_and_prefix_length_general(
    void (*oracle)(std::ostream &, std::istream &),
    const unsigned int minBlockSize,
    const unsigned int tolerance,
    const unsigned int stability)
{
    auto prevJump = find_block_size_and_prefix_length(oracle,
                                                      minBlockSize,
                                                      tolerance,
                                                      stability);
    auto blockSize = 0u;
    for (auto okay = 1u; okay <= stability; ++okay) {
        auto jump =
            find_block_size_and_prefix_length(oracle,
                                              minBlockSize + prevJump.first,
                                              tolerance,
                                              stability);
        if (!blockSize)
            blockSize = jump.first - prevJump.first;
        else if (std::abs(static_cast<int>(jump.first - prevJump.first)
                          - static_cast<int>(blockSize))
                 > tolerance)
            blockSize = okay = 0u;
        prevJump = jump;
    }
    return std::make_pair(blockSize, prevJump.second);
}


std::string forge_admin_profile_cbc()
{
    unsigned int blockSize, prefixLength;
    std::tie(blockSize, prefixLength) =
        find_block_size_and_prefix_length_general(
            [](std::ostream & out, std::istream & in) {
                std::string inStr;
                std::getline(in, inStr);
                out << generate_encrypted_cookie(inStr);
            });
    auto inputBlockIdx = prefixLength / blockSize;
    auto inputIdxInBlock = prefixLength % blockSize;
    static char inputWithNuls[] = "\0admin\0true\0";
    std::string input(inputWithNuls, sizeof inputWithNuls - 1);
    if (!inputBlockIdx) {
        input = std::string(blockSize, 'X') + input;
        ++inputBlockIdx;
    }
    auto inputPosInPrevBlock = blockSize * (inputBlockIdx - 1)
                               + inputIdxInBlock;
    auto ret = generate_encrypted_cookie(input);
    ret[inputPosInPrevBlock] ^= ';';
    ret[inputPosInPrevBlock + sizeof "\0admin"  - 1] ^= '=';
    ret[inputPosInPrevBlock + sizeof "\0admin\0true" - 1] ^= ';';
    return ret;
}

}  // namespace cryptopals
