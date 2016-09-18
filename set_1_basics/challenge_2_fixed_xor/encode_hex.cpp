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

}  // close namespace cryptopals
