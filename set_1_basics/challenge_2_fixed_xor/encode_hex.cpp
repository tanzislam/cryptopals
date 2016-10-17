#include "encode_hex.hpp"
#include <boost/io/ios_state.hpp>
#include <ios>

namespace cryptopals {

std::ostream & operator<<(std::ostream & output,
                          const encode_hex & manipulator)
{
    boost::io::ios_flags_saver flagsSaver(output,
                                          std::ios_base::hex
                                                | std::ios_base::right);
    boost::io::ios_fill_saver fillSaver(output, '0');
    output.width(2);
    output << std::noshowbase << int(manipulator.d_value);
    return output;
}


encode_hex_streambuf::encode_hex_streambuf(std::ostream & outputStream)
    : d_outputStream(outputStream)
{ }


std::streambuf::int_type
        encode_hex_streambuf::overflow(std::streambuf::int_type value)
{
    if (traits_type::eq_int_type(value, traits_type::eof())) return 1;
    try {
        return d_outputStream << encode_hex(value) ? 1 : traits_type::eof();
    } catch (...) {
        return traits_type::eof();
    }
}

}  // close namespace cryptopals
