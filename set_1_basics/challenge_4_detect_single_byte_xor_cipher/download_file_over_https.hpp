#ifndef INCLUDED_DOWNLOAD_FILE_OVER_HTTPS
#define INCLUDED_DOWNLOAD_FILE_OVER_HTTPS

#include <string>

namespace cryptopals {

std::string downloadFileOverHttps(const char * domain, const char * path);

}  // namespace cryptopals

#endif
