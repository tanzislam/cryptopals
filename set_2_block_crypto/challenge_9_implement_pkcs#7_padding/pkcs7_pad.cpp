#include "pkcs7_pad.hpp"
#include <boost/io/ios_state.hpp>
#include <cstring>

namespace cryptopals {

pkcs7_pad_streambuf::pkcs7_pad_streambuf(std::istream & inputStream,
                                         unsigned int blockSize)
    : d_inputStream(&inputStream),
      d_buffer(0),
      d_paddingBuffer(nullptr),
      d_inputLength(0u),
      d_blockSize(blockSize)
{
    if (!*d_inputStream || inputStream.tellg() != std::streampos(0))
        throw std::ios_base::failure("Cannot use already-read inputStream");
    else if (d_blockSize <= 1)
        throw std::ios_base::failure("Block size must be greater than 1");
    setg(&d_buffer, nullptr, nullptr);
}


std::streambuf::pos_type pkcs7_pad_streambuf::seekpos(
        std::streambuf::pos_type pos,
        std::ios_base::openmode which
)
{
    return
            pos == std::streampos(0)
            && which == std::ios_base::in
            && (d_inputStream->clear(), d_inputStream->seekg(pos))
        ? (
                delete [] d_paddingBuffer,
                d_paddingBuffer = nullptr,
                setg(&d_buffer, nullptr, nullptr),
                d_inputLength = 0,
                pos
        )
        : std::streambuf::seekpos(pos, which);
}


std::streambuf::pos_type pkcs7_pad_streambuf::seekoff(
        std::streambuf::off_type off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which
)
{
    return
            off == 0
            && dir == std::ios_base::cur
            && which == std::ios_base::in
        ? (
                d_paddingBuffer
                ? std::streampos(d_inputLength + gptr() - d_paddingBuffer)
                : std::streampos(d_inputStream->tellg()
                                 - std::streamoff(egptr() - gptr()))
        )
        : std::streambuf::seekoff(off, dir, which);
}


std::streambuf::int_type pkcs7_pad_streambuf::underflow()
{
    if (d_paddingBuffer)
        return std::streambuf::traits_type::eof();

    try {
        boost::io::ios_exception_saver exceptionsSaver(*d_inputStream,
                                                       std::ios_base::goodbit);
        if (d_inputStream->get(d_buffer)) {
            ++d_inputLength;
            setg(&d_buffer, &d_buffer, &d_buffer + sizeof(d_buffer));
            return std::streambuf::traits_type::to_int_type(d_buffer);
        } else if (d_inputStream->eof()) {
            char paddingBufferSize = d_blockSize % d_inputLength;
            if (paddingBufferSize == 0)
                return std::streambuf::traits_type::eof();
            d_paddingBuffer = new char[paddingBufferSize];
            std::memset(d_paddingBuffer, paddingBufferSize, paddingBufferSize);
            setg(d_paddingBuffer,
                 d_paddingBuffer,
                 d_paddingBuffer + paddingBufferSize);
            return std::streambuf::traits_type::to_int_type(*d_paddingBuffer);
        } else return std::streambuf::traits_type::eof();
    } catch (...) {
        return std::streambuf::traits_type::eof();
    }
}


pkcs7_pad_streambuf::~pkcs7_pad_streambuf()
{
    delete [] d_paddingBuffer;
}


}  // close namespace cryptopals
