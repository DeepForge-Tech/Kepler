#ifndef DEFAULT_MIDDLEWARES_HPP
#define DEFAULT_MIDDLEWARES_HPP

#include <HttpServer/Middleware.hpp>
#include <HttpServer/HttpRequest.hpp>
#include <HttpServer/HttpResponse.hpp>

namespace Kepler
{
    class DefaultMiddlewares
    {
    public:
        DefaultMiddlewares(Logger::Logging &logger)
            : logger(logger), logThread(&Logger::Logging::processFormattedLogBuffer, &logger) {}

        ~DefaultMiddlewares()
        {
            logger.setFinished(true); // Set the end flag for a thread
            logger.notifyBuffer();    // Waking up the thread to complete
            if (logThread.joinable())
            {
                logThread.join(); // Waiting for thread to complete
            }
        }

        virtual void logMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const;

        virtual void corsMiddleware(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const;

    private:
        
        Logger::Logging &logger;
        std::thread logThread;
    };
}

#endif // DEFAULT_MIDDLEWARES_HPP
