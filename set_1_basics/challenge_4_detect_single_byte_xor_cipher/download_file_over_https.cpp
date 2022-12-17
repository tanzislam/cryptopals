#include "download_file_over_https.hpp"
#if __cplusplus < 201703L
# define BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT
#endif
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/scope_exit.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>
#include <istream>

#ifdef BOOST_ASIO_WINDOWS
#    include <openssl/ssl.h>
#    include <wincrypt.h>
#    include <boost/system/system_error.hpp>
#    include <iostream>
#    ifdef _MSC_VER
#        pragma comment(lib, "crypt32")
#    endif
static void addWindowsRootCertificatesToOpenSSL(SSL_CTX * ctx)
{
    // https://stackoverflow.com/questions/9507184 (CAPI Engine didn't work)
    namespace sys = boost::system;

    auto store = CertOpenSystemStore(0, TEXT("ROOT"));
    if (!store)
        throw sys::system_error(
            sys::error_code(GetLastError(), sys::system_category()));
    BOOST_SCOPE_EXIT_ALL(&store) { CertCloseStore(store, 0); };

    for (PCCERT_CONTEXT certificate = NULL;
         (certificate = CertEnumCertificatesInStore(store, certificate));
         /*nothing*/) {
        auto encoded =
            reinterpret_cast<const BYTE *>(certificate->pbCertEncoded);
        auto x509 = d2i_X509(NULL, &encoded, certificate->cbCertEncoded);
        if (!x509) {
            std::cerr << "WARNING: Could not decode certificate: "
                      << std::string(reinterpret_cast<const char *>(
                                         certificate->pCertInfo->Subject
                                             .pbData),
                                     certificate->pCertInfo->Subject.cbData)
                      << '\n';
            continue;
        }
        BOOST_SCOPE_EXIT_ALL(&x509) { X509_free(x509); };

        if (!X509_STORE_add_cert(SSL_CTX_get_cert_store(ctx), x509))
            std::cerr << "WARNING: Could not add certificate: "
                      << std::string(reinterpret_cast<const char *>(
                                         certificate->pCertInfo->Subject
                                             .pbData),
                                     certificate->pCertInfo->Subject.cbData)
                      << '\n';
    }
}
#endif

namespace cryptopals {

std::string downloadFileOverHttps(const char * domain, const char * path)
{
    // Boost.Asio is lacking a proper IOStreams interface for SSL. See:
    //  - https://stackoverflow.com/questions/3668128
    //  - https://stackoverflow.com/questions/54311116
    namespace ssl = boost::asio::ssl;
    using tcp = boost::asio::ip::tcp;

    ssl::context sslContext(ssl::context::tlsv12_client);
    sslContext.set_default_verify_paths();
#ifdef BOOST_ASIO_WINDOWS
    addWindowsRootCertificatesToOpenSSL(sslContext.native_handle());
#endif

    boost::asio::io_context io;
    ssl::stream<tcp::socket> sslSocket(io, sslContext);

    boost::asio::connect(sslSocket.next_layer(),
                         tcp::resolver(io).resolve(domain, "https"));
    sslSocket.lowest_layer().set_option(tcp::no_delay(true));

    SSL_set_tlsext_host_name(sslSocket.native_handle(), domain);
    sslSocket.set_verify_mode(ssl::verify_peer);
    sslSocket.set_verify_callback(ssl::host_name_verification(domain));
    sslSocket.handshake(ssl::stream<tcp::socket>::client);
    BOOST_SCOPE_EXIT_ALL(&sslSocket)
    {
        sslSocket.shutdown();
        sslSocket.next_layer().close();
    };

    auto request = std::string("GET ") + path + "\r\n";
    boost::asio::write(sslSocket,
                       boost::asio::buffer(request.data(), request.size()));

    boost::asio::streambuf responseBuffer;
    try {
        boost::asio::read_until(sslSocket, responseBuffer, '\0');
    } catch (boost::system::system_error & e) {
        if (e.code() != boost::asio::error::eof) throw;
    }

    std::string fileContents;
    std::istream response(&responseBuffer);
    std::getline(response, fileContents, '\0');
    return fileContents;
}

}  // namespace cryptopals
