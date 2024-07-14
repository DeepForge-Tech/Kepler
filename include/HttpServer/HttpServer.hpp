#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <HttpServer/Session.hpp>
#include <HttpServer/Router.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class HttpServer {
public:
    HttpServer(asio::io_context& ioc, short port, const Router& router) : acceptor(ioc, tcp::endpoint(tcp::v6(), port)), router(router) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor.async_accept(
            [this](boost::beast::error_code ec, tcp::socket socket) {
                if (!ec) {
                    auto session = std::make_shared<Session>(std::move(socket), router);
                    session->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor;
    const Router& router;
};

#endif // HTTP_SERVER_HPP
