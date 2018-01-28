// getenv() is classified as unsafe by Microsoft due to the thread-safety issue
// if another thread modifies that variable using setenv(), unsetenv() or
// putenv(). We have no such issues for this code (this is the only point in our
// code that reads these environment variables, and they are never modified), so
// ignoring the warning here.
#define _CRT_SECURE_NO_WARNINGS

#include "hunspell_singleton.hpp"
#include <cstdlib>
#include <stdexcept>

namespace cryptopals {

namespace {

const char * getAffixPath()
{
    const char * affixPath = std::getenv("HUNSPELL_AFFIX_PATH");
    if (!affixPath)
        throw std::invalid_argument("Environment variable HUNSPELL_AFFIX_PATH "
                                    "must be set to the location of a Hunspell "
                                    ".aff file");
    return affixPath;
}


const char * getDictionaryPath()
{
    const char * dictionaryPath = std::getenv("HUNSPELL_DICT_PATH");
    if (!dictionaryPath)
        throw std::invalid_argument("Environment variable HUNSPELL_DICT_PATH "
                                    "must be set to the location of a Hunspell "
                                    ".dic file");
    return dictionaryPath;
}

}  // namespace


Hunspell & HunspellSingleton::inst()
{
    static Hunspell hunspell(getAffixPath(), getDictionaryPath());
    return hunspell;
}

}  // namespace cryptopals
