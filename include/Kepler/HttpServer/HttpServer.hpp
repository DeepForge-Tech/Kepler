#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <Kepler/HttpServer/Session.hpp>
#include <Kepler/HttpServer/Router.hpp>
#include <Kepler/HttpServer/DefaultMiddlewares.hpp>

#define ERROR_USING_TWO_LOGGERS "Not using base logger and custom logger."

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

namespace Kepler
{
    class HttpServer {
    public:
        HttpServer(asio::io_context &ioc, const std::string address, Router &router, uint16_t port = 80)
            : acceptor(ioc, 
                        tcp::endpoint(asio::ip::make_address_v4(address),port)), 
                        router(router), 
                        ioc(ioc),
                        logger_ptr(nullptr),  // Initialize logger pointer
                        default_middlewares_ptr(nullptr)  // Initialize default_middlewares pointer
        {}

        ~HttpServer()
        {
            if (setLogger)
            {
                logger_ptr->setFinished(true); // Set the end flag for a thread
                logger_ptr->notifyBuffer();    // Waking up the thread to complete
                if (logThread.joinable())
                {
                    logThread.join(); // Waiting for thread to complete
                }
            }
        }

        HttpServer& useBaseLogger();

        template<typename ClassType>
        HttpServer& useCustomLogger(ClassType middlewareClass);

        HttpServer& setThreadNum(uint8_t num_threads);

        HttpServer& multithread();

        HttpServer& useCors();

        HttpServer& setLogPath(const std::string path);
        
        HttpServer& setLogSize(const std::string size);

        void listen();

        void start();

    private:
        void startMiddlewares();

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

        bool setLogger = false;
        bool setMultithread = false;
        bool setCors = false;
        uint8_t numThreads = 1;
        uint8_t maxThreads = std::thread::hardware_concurrency();
        std::string logSize = "1MB";
        std::string logPath = fmt::format("{}/MainLog.log",std::filesystem::current_path().generic_string());
        tcp::acceptor acceptor;
        Router &router;
        asio::io_context &ioc;
        std::shared_ptr<Logger::Logging> logger_ptr; // Change from reference to pointer
        std::thread logThread;
        std::shared_ptr<DefaultMiddlewares> default_middlewares_ptr; // Change from reference to pointer
    };
}

#endif // HTTP_SERVER_HPP
