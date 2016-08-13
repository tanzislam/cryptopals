#ifndef INCLUDED_REPEATING_STRINGBUF
#define INCLUDED_REPEATING_STRINGBUF

#include <sstream>

namespace cryptopals {

/** @warning Do not extract text from an std::istringstream connected to an
             object of this class without setting a width (e.g. using the
             @c std::setw manipulator)!**/
class repeating_stringbuf
    : public std::stringbuf
{
  public:
    repeating_stringbuf(std::ios_base::openmode which =
                                std::ios_base::in | std::ios_base::out)
            : std::stringbuf(which) { }

    repeating_stringbuf(const std::string & new_str,
                        std::ios_base::openmode which =
                                std::ios_base::in | std::ios_base::out)
            : std::stringbuf(new_str, which) { }

    repeating_stringbuf(const repeating_stringbuf &) = delete;

    repeating_stringbuf(repeating_stringbuf && rhs)
            : std::stringbuf(std::move(rhs)) { }

  protected:
    virtual int_type underflow();
};


class repeating_stringstream
    : public std::stringstream
{
    repeating_stringbuf d_streamBuf;
    std::streambuf * d_oldStreamBuf;

  public:
    repeating_stringstream();
    ~repeating_stringstream();
};

}  // close namespace cryptopals

#endif
