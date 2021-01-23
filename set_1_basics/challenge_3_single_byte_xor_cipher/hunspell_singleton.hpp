#ifndef INCLUDED_HUNSPELL_SINGLETON
#define INCLUDED_HUNSPELL_SINGLETON

#include <boost/core/noncopyable.hpp>
#include <hunspell.hxx>

namespace cryptopals {

class HunspellSingleton : private boost::noncopyable
{
    HunspellSingleton();
    ~HunspellSingleton();

  public:
    static Hunspell & inst();
};

}  // namespace cryptopals

#endif
