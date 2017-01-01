#include "pkcs7_unpad.hpp"
#include <boost/io/ios_state.hpp>
#include <algorithm>

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
        std::ios_base::openmode which
)
{
    return
            pos == std::streampos(0)
            && which == std::ios_base::in
            && (d_inputStream->clear(), d_inputStream->seekg(pos))
        ? (resetInputBlock(), pos)
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type pkcs7_unpad_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    return
            off == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
        ? d_inputStream->rdbuf()->pubseekoff(off, dir, which)
                - std::streamoff(d_currentBlock + d_blockSize - gptr())
        : std::streambuf::seekoff(off, dir, which);
}


char * pkcs7_unpad_streambuf::findFirstPaddingByte() const
{
    unsigned int numPaddingBytes = d_currentBlock[d_blockSize - 1];
    if (numPaddingBytes > d_blockSize)
        throw std::ios_base::failure("Invalid PKCS#7 padding byte");

    char * firstPaddingByte =
            d_currentBlock + d_blockSize - numPaddingBytes;
    if (std::any_of(firstPaddingByte,
                    d_currentBlock + d_blockSize,
                    [numPaddingBytes](char c){
                        return unsigned(c) != numPaddingBytes;
                    }))
        throw std::ios_base::failure("Inconsistent PKCS#7 padding");

    return firstPaddingByte;
}


std::streambuf::int_type pkcs7_unpad_streambuf::underflow()
{
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
    : d_blockSize(blockSize), d_inputStream(0), d_outputStream(&outputStream)
{
    if (d_blockSize <= 1)
        throw std::ios_base::failure("Block size must be greater than 1");
    d_currentBlock = new char[d_blockSize];
    resetOutputBlock();
}


std::streambuf::int_type pkcs7_unpad_streambuf::overflow(int_type ch)
{
    if (ch == traits_type::eof())
        return sync() ? traits_type::eof() : !traits_type::eof();

    try {
        boost::io::ios_exception_saver exceptionsSaver(*d_outputStream,
                                                       std::ios_base::goodbit);
        if (!d_outputStream->write(pbase(), pptr() - pbase()))
            throw std::ios_base::failure("Failed to write to output stream");
        resetOutputBlock();
        sputc(ch);
        return !traits_type::eof();
    } catch (...) {
        return traits_type::eof();
    }
}


int pkcs7_unpad_streambuf::sync()
{
    try {
        if (pptr() != epptr())
            throw std::ios_base::failure("Sync after incomplete block");
        boost::io::ios_exception_saver exceptionsSaver(*d_outputStream,
                                                       std::ios_base::goodbit);
        if (!d_outputStream->write(pbase(), findFirstPaddingByte() - pbase()))
            throw std::ios_base::failure("Failed to write to output stream");
        resetOutputBlock();
        return 0;
    } catch (...) {
        return -1;
    }
}


pkcs7_unpad_streambuf::~pkcs7_unpad_streambuf()
{
    if (d_outputStream) sync();
    delete [] d_currentBlock;
}

}  // close namespace cryptopals
