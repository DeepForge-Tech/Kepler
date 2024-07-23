#ifndef HTTPS_SERVER_HPP
#define HTTPS_SERVER_HPP

#include <HttpsServer/Session.hpp>
#include <HttpsServer/Router.hpp>
#include <boost/asio/ssl.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class HttpsServer {
public:
    HttpsServer(asio::io_context& ioc, asio::ssl::context& ssl_context, uint16_t port, Router& router)
        : acceptor(ioc, tcp::endpoint(tcp::v6(), port)), ssl_context(ssl_context), router(router) {
        do_accept();
    }

    void useLogger(bool enable) {
        if (enable) {
            router.use(Middleware::logMiddleware);
        }
    }

private:
    void do_accept() {
        acceptor.async_accept(
            [this](boost::beast::error_code ec, tcp::socket socket) {
                if (!ec) {
                    auto session = std::make_shared<Session>(std::move(socket), ssl_context, router);
                    session->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor;
    asio::ssl::context& ssl_context;
    Router& router;
};

#endif // HTTPS_SERVER_HPP
