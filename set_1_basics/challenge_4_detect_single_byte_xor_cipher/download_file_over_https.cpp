#include "download_file_over_https.hpp"
#if __cplusplus < 201703L
# define BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT
#endif
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/asio/ssl/host_name_verification.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core/flat_buffer.hpp>

#ifdef BOOST_ASIO_WINDOWS
#    include <openssl/ssl.h>
#    include <wincrypt.h>
#    include <boost/system/system_error.hpp>
#    include <boost/scope_exit.hpp>
#    include <iostream>
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
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream(io, sslContext);

    stream.next_layer().connect(tcp::resolver(io).resolve(domain, "https"));
    stream.next_layer().socket().set_option(tcp::no_delay(true));

    SSL_set_tlsext_host_name(stream.native_handle(), domain);
    stream.set_verify_mode(ssl::verify_peer);
    stream.set_verify_callback(ssl::host_name_verification(domain));
    stream.handshake(ssl::stream<tcp::socket>::client);

    namespace http = boost::beast::http;
    http::request<http::empty_body> request(http::verb::get, path, 11);
    request.set(http::field::host, domain);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::accept, "*/*");

    http::write(stream, request);

    boost::beast::flat_buffer buffer;
    http::response<http::string_body> response;
    http::read(stream, buffer, response);

    try {
        stream.shutdown();
    } catch (const boost::system::system_error & e) {
        // https://github.com/boostorg/beast/issues/38
        if (e.code() != ssl::error::stream_truncated) throw;
    }
    stream.next_layer().socket().shutdown(tcp::socket::shutdown_send);

    if (response.result() != http::status::ok)
        throw std::runtime_error(
                "GET failed: "
                + std::to_string(static_cast<unsigned int>(response.result()))
                + ": " + std::string(response.reason()));
    return std::move(response.body());
}

}  // namespace cryptopals
