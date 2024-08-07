#ifndef MIDDLEWARE_HPP
#define MIDDLEWARE_HPP

#include <Kepler/HttpServer/HttpRequest.hpp>
#include <Kepler/HttpServer/HttpResponse.hpp>

namespace Kepler
{
    class Middleware {
    public:
        using MiddlewareFunction = std::function<void(const HttpRequest&, HttpResponse&, std::function<void()>)>;

        void add(MiddlewareFunction middleware);

        void handle(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const;

    private:
        void do_handle(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const;

        mutable size_t index = 0;
        std::vector<MiddlewareFunction> middlewares;
    };
}

#endif // MIDDLEWARE_HPP
