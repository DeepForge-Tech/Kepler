#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <HttpServer/Session.hpp>
#include <HttpServer/Router.hpp>
#include <HttpServer/DefaultMiddlewares.hpp>
#include <memory>

#define ERROR_USING_TWO_LOGGERS "Not using base logger and custom logger."

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace Kepler
{
    class HttpServer {
    public:
        HttpServer(asio::io_context& ioc, const asio::ip::address_v4& address, uint16_t port, Router& router)
            : acceptor(ioc, tcp::endpoint(address, port)), router(router) {}

        HttpServer& useBaseLogger();

        HttpServer& useCustomLogger(Middleware::MiddlewareFunction log_middleware);


        HttpServer& listen();

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

        bool setBaseLogger = false;
        bool setCustomLogger = false;
        tcp::acceptor acceptor;
        Router& router;
        std::shared_ptr<Logger::Logging> logger;
        std::thread logThread;
        std::shared_ptr<DefaultMiddlewares> default_middlewares;
    };
}

#endif // HTTP_SERVER_HPP
