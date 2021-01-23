#ifndef INCLUDED_AES_CTR
#define INCLUDED_AES_CTR

#include <streambuf>
#include <boost/core/noncopyable.hpp>
#include <istream>
#include <string>
#include <cstdint>
#include "disable_method.hpp"

namespace cryptopals {

class aes_ctr_streambuf : public std::streambuf, private boost::noncopyable
{
    std::istream & d_inputStream;
    char d_inputBuffer;

    std::string d_key;
    std::uint64_t d_nonce;
    std::uint64_t d_counter;
    std::string d_encryptedNonceAndCounter;

    void setupNextBlock();

  protected:
    int_type underflow() override;

    DISABLE_VOID_METHOD(void imbue(const std::locale &))
    DISABLE_METHOD(std::streambuf * setbuf(char_type *, std::streamsize))
    DISABLE_METHOD(pos_type seekoff(off_type,
                                    std::ios_base::seekdir,
                                    std::ios_base::openmode))
    DISABLE_METHOD(pos_type seekpos(pos_type, std::ios_base::openmode))
    DISABLE_METHOD(int sync())
    DISABLE_METHOD(std::streamsize showmanyc())
    DISABLE_METHOD(int_type overflow(int_type))
    DISABLE_METHOD(int_type pbackfail(int_type))

  public:
    aes_ctr_streambuf(std::istream & input,
                      const std::string & key,
                      std::uint64_t nonce);
};

}  // namespace cryptopals

#endif
