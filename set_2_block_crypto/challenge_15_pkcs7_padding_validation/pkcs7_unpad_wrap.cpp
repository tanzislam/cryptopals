#include "pkcs7_unpad_wrap.hpp"
#include <stdexcept>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include "pkcs7_unpad.hpp"

namespace cryptopals {

std::string unpad_pkcs7(const std::string & input)
{
    if (input.empty() || static_cast<unsigned int>(input.back()) > input.size())
        throw std::invalid_argument("Input empty or shorter than PKCS#7 byte");

    boost::iostreams::stream<boost::iostreams::array_source>
        instr(input.c_str(), input.size());

    pkcs7_unpad_streambuf unpadder(instr, input.size());
    std::istream unpadStream(&unpadder);
    // Exception on failbit should be enough, but Clang/libc++ demands badbit.
    // https://stackoverflow.com/questions/35019687
    unpadStream.exceptions(std::istream::failbit | std::istream::badbit);

    std::string output;
    std::getline(unpadStream, output, '\0');
    return output;
}

}  // namespace cryptopals
