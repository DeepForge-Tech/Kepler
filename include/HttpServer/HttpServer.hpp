#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <HttpServer/Session.hpp>
#include <HttpServer/Router.hpp>
#include <HttpServer/DefaultMiddlewares.hpp>
#include <memory>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace Kepler
{
    class HttpServer {
    public:
        HttpServer(asio::io_context& ioc, uint16_t port, Router& router)
            : acceptor(ioc, tcp::endpoint(tcp::v4(), port)), router(router) {}

        HttpServer& setLogger(bool enable) {
            if (enable) {
                logger = std::make_shared<Logger::Logging>("Kepler.log", "10mb");
                logThread = std::thread(&Logger::Logging::processLogBuffer, logger.get());
                default_middlewares = std::make_shared<DefaultMiddlewares>(*logger);
                // router.use(default_middlewares->getLogMiddleware());
                router.use(std::bind(&DefaultMiddlewares::logMiddleware, default_middlewares.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            }
            this->useLogger = enable;
            return *this;
        }

        HttpServer& listen() {
            do_accept();
            return *this;
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

        bool useLogger = false;
        tcp::acceptor acceptor;
        Router& router;
        std::shared_ptr<Logger::Logging> logger;
        std::thread logThread;
        std::shared_ptr<DefaultMiddlewares> default_middlewares;
    };
}

#endif // HTTP_SERVER_HPP
