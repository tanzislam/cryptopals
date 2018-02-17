#ifndef INCLUDED_ECB_ENCRYPTION_ORACLE
#define INCLUDED_ECB_ENCRYPTION_ORACLE

#include <ostream>
#include <istream>

namespace cryptopals {

void ecbEncryptOracle(std::ostream & outputStream, std::istream & inputStream);

}  // namepace cryptopals

#endif
