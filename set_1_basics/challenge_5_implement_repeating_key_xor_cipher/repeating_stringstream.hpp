#ifndef INCLUDED_REPEATING_STRINGBUF
#define INCLUDED_REPEATING_STRINGBUF

#include <sstream>
#include <boost/core/noncopyable.hpp>

namespace cryptopals {

/** @warning Do not extract text from an object of this class without setting a
             width (e.g. using the @c std::setw manipulator)!**/
class repeating_stringstream : public std::stringstream,
                               private boost::noncopyable
{
    class repeating_stringbuf : public std::stringbuf
    {
        virtual int_type underflow() override;

      public:
        explicit repeating_stringbuf(std::ios_base::openmode which =
                                         std::ios_base::in | std::ios_base::out)
                : std::stringbuf(which)
        {}
    };

    repeating_stringbuf d_streamBuf;
    std::streambuf * d_oldStreamBuf;

  public:
    repeating_stringstream();
    ~repeating_stringstream();
};

}  // namespace cryptopals

#endif
