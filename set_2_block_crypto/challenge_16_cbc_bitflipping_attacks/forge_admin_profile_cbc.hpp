#ifndef INCLUDED_FORGE_ADMIN_PROFILE_CBC
#define INCLUDED_FORGE_ADMIN_PROFILE_CBC

#include <ostream>
#include <istream>
#include <utility>
#include <string>

namespace cryptopals {

// This is a wrapper over the version in Challenge 14, that works even if the
// prefix is longer than the block size.
std::pair<unsigned int, unsigned int> find_block_size_and_prefix_length_general(
    void (*oracle)(std::ostream &, std::istream &),
    const unsigned int minBlockSize = 3u,
    const unsigned int tolerance = 3u,
    const unsigned int stability = 3u);

std::string forge_admin_profile_cbc();

}  // namespace cryptopals

#endif
