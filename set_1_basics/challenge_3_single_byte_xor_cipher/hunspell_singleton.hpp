#ifndef INCLUDED_HUNSPELL_SINGLETON
#define INCLUDED_HUNSPELL_SINGLETON

#include <hunspell.hxx>

namespace cryptopals {

class HunspellSingleton
{
    HunspellSingleton();
    HunspellSingleton(const HunspellSingleton &);
    HunspellSingleton & operator=(const HunspellSingleton&);
    ~HunspellSingleton();

  public:
    static Hunspell & inst();
};

}  // close namespace cryptopals

#endif
