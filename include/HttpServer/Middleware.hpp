#ifndef MIDDLEWARE_HPP
#define MIDDLEWARE_HPP

#include <HttpServer/HttpRequest.hpp>
#include <HttpServer/HttpResponse.hpp>

namespace Kepler
{
    class Middleware {
    public:
        using MiddlewareFunction = std::function<void(const HttpRequest&, HttpResponse&, std::function<void()>)>;

        // Middleware() : logger(logger) {}

        void add(MiddlewareFunction middleware) {
            middlewares.push_back(std::move(middleware));
        }

        void handle(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const {
            index = 0;
            do_handle(req, res, next);
        }

    private:
        void do_handle(const HttpRequest& req, HttpResponse& res, std::function<void()> next) const {
            if (index < middlewares.size()) {
                middlewares[index++](req, res, [this, &req, &res, next] {
                    do_handle(req, res, next);
                });
            } else {
                next();
            }
        }

        mutable size_t index = 0;
        std::vector<MiddlewareFunction> middlewares;
    };
}

#endif // MIDDLEWARE_HPP
