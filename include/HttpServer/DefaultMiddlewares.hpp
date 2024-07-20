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
            : logger(logger), logThread(&Logger::Logging::processLogBuffer, &logger) {}

        ~DefaultMiddlewares()
        {
            logger.setFinished(true); // Устанавливаем флаг завершения для потока
            logger.notifyBuffer();    // Пробуждаем поток для завершения
            if (logThread.joinable())
            {
                logThread.join(); // Ожидание завершения потока
            }
        }

        void logMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const;


    private:
        
        Logger::Logging &logger;
        std::thread logThread;
    };
}

#endif // DEFAULT_MIDDLEWARES_HPP
