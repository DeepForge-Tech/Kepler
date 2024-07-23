#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <HttpServer/Session.hpp>
#include <HttpServer/Router.hpp>
#include <HttpServer/DefaultMiddlewares.hpp>

#define ERROR_USING_TWO_LOGGERS "Not using base logger and custom logger."

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace Kepler
{
    class HttpServer {
    public:
        HttpServer(asio::io_context& ioc, const std::string address, Router& router, uint16_t port = 80)
            : acceptor(ioc, 
                        tcp::endpoint(asio::ip::make_address_v4(address),port)), 
                        router(router), 
                        ioc(ioc)
        {}

        HttpServer& useBaseLogger();

        HttpServer& useCustomLogger(Middleware::MiddlewareFunction log_middleware);

        HttpServer& setThreadNum(uint8_t num_threads);

        HttpServer& multithread();

        HttpServer& useCors();

        HttpServer& setLogPath(const std::string path);
        
        HttpServer& setLogSize(const std::string size);

        void listen();

        void start();

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
        bool setMultithread = false;
        uint8_t numThreads = 1;
        uint8_t maxThreads = std::thread::hardware_concurrency();
        std::string logSize = "1MB";
        std::string logPath = fmt::format("{}/MainLog.log",std::filesystem::current_path().generic_string());
        tcp::acceptor acceptor;
        Router& router;
        asio::io_context& ioc;
        std::shared_ptr<Logger::Logging> logger;
        std::thread logThread;
        std::shared_ptr<DefaultMiddlewares> default_middlewares;
    };
}

#endif // HTTP_SERVER_HPP
