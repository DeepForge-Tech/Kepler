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

        Middleware::MiddlewareFunction getLogMiddleware() const
        {
            return [this](const HttpRequest &req, HttpResponse &res, std::function<void()> next)
            {
                logMiddleware(req, res, next);
            };
        }
        void logMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const
        {
            std::stringstream ss;
            ss << std::this_thread::get_id();
            std::string threadIdStr = std::move(ss.str());
            ss.str().resize(0); // савмый стопудовый способ
            ss.str().clear();
            ss.str("");
            ss << req.method();
            std::string method = std::move(ss.str());
            std::string logText = fmt::format("Thread: {} | {} {} {}", threadIdStr, method, req.target(), std::to_string(res.result_int()));
            logger.addLogToBuffer(logText);
            next();
        }


    private:
        
        Logger::Logging &logger;
        std::thread logThread;
    };
}

#endif // DEFAULT_MIDDLEWARES_HPP
