#include "pkcs7_unpad.hpp"
#include <boost/io/ios_state.hpp>
#include <algorithm>
#include <cassert>
#include <boost/scope_exit.hpp>

namespace cryptopals {

void pkcs7_unpad_streambuf::resetInputBlock()
{
    setg(d_currentBlock,
         d_currentBlock + d_blockSize,
         d_currentBlock + d_blockSize);
}


pkcs7_unpad_streambuf::pkcs7_unpad_streambuf(std::istream & inputStream,
                                             unsigned int blockSize)
        : d_blockSize(blockSize), d_inputStream(&inputStream), d_outputStream(0)
{
    if (!*d_inputStream || d_inputStream->tellg() != std::streampos(0))
        throw std::ios_base::failure("Cannot use already-read inputStream");
    else if (d_blockSize <= 1)
        throw std::ios_base::failure("Block size must be greater than 1");
    d_currentBlock = new char[d_blockSize];
    resetInputBlock();
}


std::streambuf::pos_type pkcs7_unpad_streambuf::seekpos(
    std::streambuf::pos_type pos,
    std::ios_base::openmode which)
{
    assert(pos == std::streampos(0));
    assert(which == std::ios_base::in);
    return pos == std::streampos(0) && which == std::ios_base::in
                   && (d_inputStream->clear(), d_inputStream->seekg(pos))
               ? (resetInputBlock(), pos)
               : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type pkcs7_unpad_streambuf::seekoff(
    std::streambuf::off_type off,
    std::ios_base::seekdir dir,
    std::ios_base::openmode which)
{
    assert(off == 0);
    assert(dir == std::ios_base::cur);
    assert(which == std::ios_base::in);
    return off == 0 && dir == std::ios_base::cur && which == std::ios_base::in
               ? d_inputStream->rdbuf()->pubseekoff(off, dir, which)
                     - std::streamoff(d_currentBlock + d_blockSize - gptr())
               : std::streambuf::seekoff(off, dir, which);
}


char * pkcs7_unpad_streambuf::findFirstPaddingByte() const
{
    assert(d_outputStream ? pptr() == epptr() : gptr() == egptr());
    unsigned int numPaddingBytes = d_currentBlock[d_blockSize - 1];
    if (numPaddingBytes == 0 || numPaddingBytes > d_blockSize)
        throw std::ios_base::failure("Invalid PKCS#7 padding byte");

    char * firstPaddingByte = d_currentBlock + d_blockSize - numPaddingBytes;
    if (std::any_of(firstPaddingByte,
                    d_currentBlock + d_blockSize,
                    [numPaddingBytes](char c) {
                        return static_cast<unsigned int>(c) != numPaddingBytes;
                    }))
        throw std::ios_base::failure("Inconsistent PKCS#7 padding");

    return firstPaddingByte;
}


std::streambuf::int_type pkcs7_unpad_streambuf::underflow()
{
    assert(!d_outputStream);
    assert((!gptr() && !egptr()) || (gptr() && egptr() && gptr() == egptr()));
    try {
        boost::io::ios_exception_saver exceptionsSaver(*d_inputStream,
                                                       std::ios_base::goodbit);

        if (!d_inputStream->read(d_currentBlock, d_blockSize))
            throw std::ios_base::failure("EOF reached after incomplete block");

        if (d_inputStream->peek() == std::istream::traits_type::eof())
            setg(d_currentBlock, d_currentBlock, findFirstPaddingByte());
        else
            setg(d_currentBlock, d_currentBlock, d_currentBlock + d_blockSize);
        return std::streambuf::traits_type::to_int_type(*d_currentBlock);
    } catch (...) {
        return std::streambuf::traits_type::eof();
    }
}


void pkcs7_unpad_streambuf::resetOutputBlock()
{
    setp(d_currentBlock, d_currentBlock + d_blockSize);
}


pkcs7_unpad_streambuf::pkcs7_unpad_streambuf(std::ostream & outputStream,
                                             unsigned int blockSize)
        : d_blockSize(blockSize),
          d_inputStream(0),
          d_outputStream(&outputStream)
{
    if (d_blockSize <= 1)
        throw std::ios_base::failure("Block size must be greater than 1");
    d_currentBlock = new char[d_blockSize];
    resetOutputBlock();
}


void pkcs7_unpad_streambuf::emitBuffer(char * end)
{
    assert(d_outputStream && end && pbase());
    d_outputStream->write(pbase(), end - pbase());
}


std::streambuf::int_type pkcs7_unpad_streambuf::overflow(int_type ch)
{
    assert(d_outputStream);
    assert((!pptr() && !epptr()) || (pptr() && epptr() && pptr() == epptr()));
    if (ch == traits_type::eof()) return !traits_type::eof();

    try {
        emitBuffer(pptr());
        resetOutputBlock();
        sputc(ch);
        return !traits_type::eof();
    } catch (...) {
        return traits_type::eof();
    }
}


pkcs7_unpad_streambuf::~pkcs7_unpad_streambuf()
{
    BOOST_SCOPE_EXIT_ALL(this) { delete[] this->d_currentBlock; };
    if (d_outputStream && pptr() != pbase()) try {
            if (pptr() != epptr())
                throw std::ios_base::failure("Sync after incomplete block");
            emitBuffer(findFirstPaddingByte());
        } catch (...) {
            d_outputStream->setstate(std::ios_base::failbit);
        }
}

}  // namespace cryptopals
