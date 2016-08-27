#include "repeating_stringstream.hpp"

namespace cryptopals {

repeating_stringstream::repeating_stringbuf::int_type
        repeating_stringstream::repeating_stringbuf::underflow()
{
    if (egptr() > gptr() || pptr() > egptr()) {
        return std::stringbuf::underflow();
    }

    // gptr() has reached egptr() and there are no extra characters in the "put
    // buffer". So reset gptr() back to eback() and return success.
    setg(eback(), eback(), egptr());
    return traits_type::to_int_type(*gptr());
}


repeating_stringstream::repeating_stringstream()
{
    d_oldStreamBuf = std::ios::rdbuf(&d_streamBuf);
}


repeating_stringstream::~repeating_stringstream()
{
    std::ios::rdbuf(d_oldStreamBuf);
}

}  // close namespace cryptopals
