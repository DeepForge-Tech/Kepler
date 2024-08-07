#ifndef DEFAULT_MIDDLEWARES_HPP
#define DEFAULT_MIDDLEWARES_HPP

#include <Kepler/HttpServer/Middleware.hpp>
#include <Kepler/HttpServer/HttpRequest.hpp>
#include <Kepler/HttpServer/HttpResponse.hpp>

namespace Kepler
{
    class DefaultMiddlewares
    {
    public:
        DefaultMiddlewares(Logger::Logging *logger_ptr = nullptr)
            : logger(logger_ptr)
        {}

        virtual void logMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const;

        virtual void corsMiddleware(const HttpRequest &req, HttpResponse &res, std::function<void()> next) const;

    private:
        Logger::Logging *logger;
    };
}

#endif // DEFAULT_MIDDLEWARES_HPP
